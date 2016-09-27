
#pragma once

#include <string>
#include <vector>
#include <unordered_map>

#include <GG/Core/Vector.h>
#include <GG/Core/Matrix.h>
#include <GG/Core/Types.h>

#include "IwGL.h"

namespace GG
{
	class Shader
	{
	public:

		Shader( );
		~Shader( );

		
		uint getId() const;


		void bind( );
		void bindAttribute(int tag, const std::string & name);

		void addDefinition( const std::string & define );

		bool compile( 
			const std::string & vertexShader, 
			const std::string & pixelShader 
		);

		void update( );

		
		void setParameter( const std::string & name, int x );
		void setParameter( const std::string & name, float x );
		void setParameter( const std::string & name, float x, float y );
		void setParameter( const std::string & name, float x, float y, float z );
		void setParameter( const std::string & name, float x, float y, float z, float w );
		void setParameter( const std::string & name, const Vector2 & v );
		void setParameter( const std::string & name, const Vector3 & v );
		void setParameter( const std::string & name, const Vector4 & v );
		void setParameter( const std::string & name, const Matrix4 & m );

		void setParameterSampler(const std::string & name, uint samplerID );

	private:
		typedef std::unordered_map< std::string, int >	ParameterMap;
		typedef std::vector< std::string >		AttributeList;
		typedef std::vector< std::string >		DefineList;

		ParameterMap		parameterHandles;
		AttributeList		attributeHandles;
		DefineList			defineList;

		uint				vertexHandle;
		uint				pixelHandle;
		uint				programHandle;

	private:
		void				_validateProgram();
		void				_bindAllAttributes();
		int					_getParameterId(const std::string & name);

		bool				loadShader(
								GLuint & shader, 
								const std::string & shaderSrc, 
								GLenum type
							);
	};
}

