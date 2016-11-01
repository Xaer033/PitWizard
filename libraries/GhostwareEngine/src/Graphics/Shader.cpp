
#include "Shader.h"


#include <map>
#include <sstream>
#include <vector>
#include <string>


#include "s3e.h"
#include "IwGL.h"

#include <GG/Core/Vector.h>
#include <GG/Core/Matrix.h>
#include <GG/Core/MathDebug.h>
#include <GG/Core/Log.h>
#include <GG/Core/Json.h>


namespace GG
{
	Shader::Shader() :
		vertexHandle(0),
		pixelHandle(0),
		programHandle(0),
		IResource()
	{
	}

	Shader::Shader(const json & j) :
		vertexHandle(0),
		pixelHandle(0),
		programHandle(0)
	{
		_descriptor = ShaderDescriptor::FromJson(j);
	}

	Shader::~Shader()
	{
		shutdown();
	}


	IResourceDescriptor * Shader::getDescriptor()
	{
		return &_descriptor;
	}

	StringId Shader::getType() const
	{
		return GetResourceType();
	}

	void Shader::init()
	{
	}

	void Shader::shutdown()
	{
		defineList.clear();
		attributeHandles.clear();
		parameterHandles.clear();

		if(vertexHandle)
		{
			glDetachShader(programHandle, vertexHandle);
			glDeleteShader(vertexHandle);
		}

		if(pixelHandle)
		{
			glDetachShader(programHandle, pixelHandle);
			glDeleteShader(pixelHandle);
		}

		if(programHandle)
			glDeleteProgram(programHandle);

		vertexHandle	= 0;
		pixelHandle		= 0;
		programHandle	= 0;
	}

	uint Shader::getId() const
	{
		return programHandle;
	}

	void Shader::bindAttribute( int tag, const std::string & name )
	{
		attributeHandles[ tag ] = name;
	}

	void Shader::bind( ) const
	{
		glUseProgram( programHandle );
	}


// Create a shader object, load the shader source, and
// compile the shader.
	void Shader::_validateProgram( )
	{
		const uint BUFFER_SIZE = 1024;
		char buffer[ BUFFER_SIZE ];
		memset( buffer, 0, BUFFER_SIZE );

		GLsizei length = 0;
		memset( buffer, 0, BUFFER_SIZE );

		glGetProgramInfoLog( programHandle, BUFFER_SIZE, &length, buffer);

		if (length > 0)
		{
			TRACE_ERROR("Program %d linker: %s", programHandle, buffer);
		}

		glValidateProgram( programHandle );

		GLint status = 0;

		glGetProgramiv( programHandle, GL_VALIDATE_STATUS, &status );
		if (status == GL_FALSE)
		{
			LOG_ERROR("Error validating shader!");
		}
	}


	bool Shader::_loadShader( uint & outShader, const std::string & shaderSrc, uint type )
	{

		if( shaderSrc.empty() ) { return false; }

		GLint compiled =  0;
		// Create the shader object
        if(outShader == 0)
			outShader = glCreateShader( type );

		std::stringstream stream;
		for(uint i = 0; i < defineList.size(); ++i)
		{
			stream << defineList[i];
		}
		stream << shaderSrc;
		const char * shaderString = shaderSrc.c_str();// stream.str().c_str();
		//TODO: get char array from vector of strings
		glShaderSource(outShader, 1, &shaderString, nullptr );

		// Compile the shader
		glCompileShader(outShader);
		// Check the compile status
		glGetShaderiv(outShader, GL_COMPILE_STATUS, &compiled );

		if( !compiled )
		{
			GLint infoLen = 0;
			glGetShaderiv(outShader, GL_INFO_LOG_LENGTH, &infoLen );

			if( infoLen > 1 )
			{
				char * infoLog = (char*)malloc( sizeof(char) * infoLen );
				glGetShaderInfoLog(outShader, infoLen, nullptr, infoLog);
					
				TRACE_ERROR("Error Compiling shader: %s", infoLog);

				free( infoLog );
			}

			glDeleteShader(outShader);
			return false;
		}
        return true;
	}


	void Shader::_bindAllAttributes()
	{
		for(auto pair : attributeHandles)
			glBindAttribLocation(programHandle, pair.first, pair.second.c_str());;
	}


	void  Shader::addDefinition( const std::string & define )
	{
		defineList.push_back( define );
	}


	bool  Shader::compile(
		const std::string & vertexShader, 
		const std::string & pixelShader )
	{
		int linked	= 0;

		bool vertSuccess	= _loadShader( vertexHandle,  vertexShader,   GL_VERTEX_SHADER    );
		bool pixelSuccess	= _loadShader( pixelHandle,   pixelShader,    GL_FRAGMENT_SHADER  );
			
		if( !vertSuccess || !pixelSuccess ) 
		{
			TRACE_ERROR("Vertex or Pixel shader did not load properly!");
			return false; 
		}

		if(programHandle == 0)
			programHandle	= glCreateProgram();
        

		glAttachShader( programHandle, vertexHandle );
		glAttachShader( programHandle, pixelHandle );

		_bindAllAttributes();

		// Link the program
		glLinkProgram( programHandle );
		// Check the link status
		glGetProgramiv( programHandle, GL_LINK_STATUS, &linked );

		if( !linked )
		{
			GLint infoLen = 0;
			glGetProgramiv( programHandle, GL_INFO_LOG_LENGTH, &infoLen );

			if( infoLen > 1 )
			{
				char* infoLog = (char*)malloc(sizeof(char) * infoLen);
				glGetProgramInfoLog( programHandle, infoLen, NULL, infoLog );

				TRACE_ERROR("Error Compiling shader: %s", infoLog);
				free( infoLog );
			}
			
			glDeleteProgram( programHandle );
			return false;
		}

			
		_validateProgram();

		return true;
	}
		
	int Shader::_getParameterId( const std::string & name )
	{
		if( programHandle == 0 ) { return -1; }

		auto it = parameterHandles.find(  name  );
		if( it == parameterHandles.end() )
		{
			int ID = glGetUniformLocation( programHandle, name.c_str() );
			parameterHandles[ name ] = ID;

			return ID;
		}

		return it->second;
	}

	void Shader::setParameter( const std::string & name, int x )
	{
		glUniform1i( _getParameterId( name ), x );
	}

	void Shader::setParameter(const std::string & name, float x )
	{
		glUniform1f( _getParameterId( name ), x );
	}

	void Shader::setParameter( const std::string & name, float x, float y )
	{
		glUniform2f( _getParameterId( name ), x, y );
	}

	void Shader::setParameter( const std::string & name, float x, float y, float z )
	{
		glUniform3f( _getParameterId( name ), x, y, z );
	}

	void Shader::setParameter( const std::string & name, float x, float y, float z, float w )
	{
		glUniform4f( _getParameterId(name), x, y, z, w );
	}


	void Shader::setParameter( const std::string & name, const Vector2 & v )
	{
		glUniform2f( _getParameterId( name ), v.x, v.y );
	}

	void Shader::setParameter( const std::string & name, const Vector3 & v )
	{
		glUniform3f( _getParameterId( name ), v.x, v.y, v.z );
	}

	void Shader::setParameter( const std::string & name, const Vector4 & v )
	{
		glUniform4f( _getParameterId( name ), v.x, v.y, v.z, v.w );
	}

	void Shader::setParameter( const std::string & name, uint unitIndex, uint samplerID )
	{
	// Set the sampler texture unit to 0
		glUniform1i( _getParameterId( name ) , unitIndex );
		glActiveTexture(GL_TEXTURE0 + unitIndex);
		glBindTexture(GL_TEXTURE_2D, samplerID);
	}


	void Shader::setParameter( const std::string & name, const Matrix4 & m )
	{
		glUniformMatrix4fv( _getParameterId( name ), 1, GL_FALSE, glm::value_ptr( m ) );
	}
}