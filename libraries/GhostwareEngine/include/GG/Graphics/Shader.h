// Ghostware Games inc. 2016  -Julian Williams
#pragma once

#include <string>
#include <vector>
#include <unordered_map>

#include <GG/Core/Vector.h>
#include <GG/Core/Matrix.h>
#include <GG/Core/Types.h>
#include <GG/Core/StringId.h>
#include <GG/Core/Json.h>
#include <GG/Resources/IResource.h>
#include "ShaderDescriptor.h"

namespace GG
{
	class Shader : public IResource
	{
	public:
		static StringId GetResourceType()
		{
			static StringId id = STRING_ID("Shader");
			return id;
		}

	public:
		Shader( );
		Shader(const json & j);
		virtual ~Shader( );

		virtual void	init() final;
		virtual void	shutdown() final;
		virtual IResourceDescriptor * getDescriptor()	final;
		virtual StringId	getType() const				final;

		uint	getId() const;


		void	bind( ) const;
		void	bindAttribute(int tag, const std::string & name);

		void	addDefinition( const std::string & define );

		bool	compile(const std::string & vertexShader, 
						const std::string & pixelShader );

		
		void	setParameter( const std::string & name, int x );
		void	setParameter( const std::string & name, float x );
		void	setParameter( const std::string & name, float x, float y );
		void	setParameter( const std::string & name, float x, float y, float z );
		void	setParameter( const std::string & name, float x, float y, float z, float w );
		void	setParameter( const std::string & name, const Vector2 & v );
		void	setParameter( const std::string & name, const Vector3 & v );
		void	setParameter( const std::string & name, const Vector4 & v );
		void	setParameter( const std::string & name, const Matrix4 & m );
		void	setParameter( const std::string & name, uint unitIndex, uint samplerID );

	private:
		typedef std::unordered_map< std::string, int >	ParameterMap;
		typedef std::vector< std::string >				AttributeList;
		typedef std::vector< std::string >				DefineList;

		ParameterMap		parameterHandles;
		AttributeList		attributeHandles;
		DefineList			defineList;

		uint				vertexHandle;
		uint				pixelHandle;
		uint				programHandle;

		ShaderDescriptor	_descriptor;

	private:
		void				_validateProgram();
		void				_bindAllAttributes();
		int					_getParameterId(const std::string & name);

		bool				_loadShader(
								uint & outShader, 
								const std::string & shaderSrc, 
								uint type );
	};
}

