
#include "Shader.h"


#include <map>
#include <sstream>
#include <vector>
#include <string>


#include "s3e.h"
#include "IwGL.h"

#include <GG/Core/Vector.h>
#include <GG/Core/Matrix.h>
#include <GG/Core/Log.h>
#include <GG/Graphics/Texture.h>


namespace GG
{
	Shader::Shader()
	{
		vertexHandle	= 0;
		pixelHandle		= 0;

		programHandle	= 0;

		for( int i = 0; i < 7; ++i )
			attributeHandles.push_back("");

	}

	Shader::~Shader()
	{
		if( vertexHandle )
			glDeleteShader( vertexHandle );

		if( pixelHandle )
			glDeleteShader( pixelHandle );

		if( programHandle )
			glDeleteProgram( programHandle );
	}


	uint Shader::getId() const
	{
		return programHandle;
	}

	void Shader::bindAttribute( int tag, const std::string & name )
	{
		if( (uint)tag < attributeHandles.size() )
			attributeHandles[ tag ] = name;
	}

	void Shader::bind( )
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
			LOG_ERROR("Program %d linker: %s", programHandle, buffer);
		}

		glValidateProgram( programHandle );

		GLint status = 0;

		glGetProgramiv( programHandle, GL_VALIDATE_STATUS, &status );
		if (status == GL_FALSE)
		{
			LOG_ERROR("Error validating shader!");
		}
	}


	bool Shader::loadShader( GLuint & shader, const std::string & shaderSrc, GLenum type )
	{

		if( shaderSrc.empty() ) { return false; }

		GLint compiled;
		// Create the shader object
        if( shader == 0 )
			shader = glCreateShader( type );

		std::stringstream stream;
		for(uint i = 0; i < defineList.size(); ++i)
		{
			stream << defineList[i];
		}
		stream << shaderSrc;
		const char * shaderString = stream.str().c_str();
		//TODO: get char array from vector of strings
		glShaderSource( shader, 1, &shaderString, NULL );	

		// Compile the shader
		glCompileShader( shader );
		// Check the compile status
		glGetShaderiv( shader, GL_COMPILE_STATUS, &compiled );

		if( !compiled )
		{
			GLint infoLen = 0;
			glGetShaderiv( shader, GL_INFO_LOG_LENGTH, &infoLen );

			if( infoLen > 1 )
			{
				char * infoLog = (char*)malloc( sizeof(char) * infoLen );
				glGetShaderInfoLog(shader, infoLen, NULL, infoLog);
					
				LOG_ERROR("Error Compiling shader: %s", infoLog);

				free( infoLog );
			}

			glDeleteShader( shader );
			return false;
		}
        return true;
	}


	void Shader::_bindAllAttributes()
	{
		for( uint i = 0; i < attributeHandles.size(); ++i )
			glBindAttribLocation( programHandle, i, attributeHandles[i].c_str() );
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

		bool vertSuccess	= loadShader( vertexHandle,  vertexShader,   GL_VERTEX_SHADER    );
		bool pixelSuccess	= loadShader( pixelHandle,   pixelShader,    GL_FRAGMENT_SHADER  );
			
		if( !vertSuccess || !pixelSuccess ) 
		{
			LOG_ERROR("Vertex or Pixel shader did not load properly!");
			return false; 
		}

		if( programHandle == 0 )
        { 
			programHandle	= glCreateProgram();
        }

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

				LOG_ERROR("Error Compiling shader: %s", infoLog);
				free( infoLog );
			}
			
			glDeleteProgram( programHandle );
			return false;
		}

			
		_validateProgram();

		return true;
	}

	void Shader::update( )
	{

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

	void Shader::setParameterSampler( const std::string & name, uint samplerID )
	{
	// Set the sampler texture unit to 0
		glUniform1i( _getParameterId( name ) , samplerID );
	}


	void Shader::setParameter( const std::string & name, const Matrix4 & m )
	{
		glUniformMatrix4fv( _getParameterId( name ), 1, GL_FALSE, glm::value_ptr( m ) );
	}
}