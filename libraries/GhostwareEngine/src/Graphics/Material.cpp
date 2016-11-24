
#include "Material.h"
#include "MaterialDescriptor.h"
#include "Shader.h"
#include "Texture2D.h"

#include <GG/Resources/ResourceHandle.h>
#include <GG/Resources/ResourceManager.h>

namespace GG
{
	Material::Material() : IResource()
	{
	}

	Material::Material(const json & j) : IResource(j)
	{
		_descriptor = MaterialDescriptor::FromJson(j);
	}

	void Material::init()
	{
	}

	void Material::shutdown()
	{
	}

	void Material::bindToShader(ResourceH<Shader> shader)
	{
		for(const auto & pair : _intUniforms)
			shader->setParameter(pair.first, pair.second);

		for(const auto & pair : _floatUniforms)
			shader->setParameter(pair.first, pair.second);

		for(const auto & pair : _vector2Uniforms)
			shader->setParameter(pair.first, pair.second);

		for(const auto & pair : _vector3Uniforms)
			shader->setParameter(pair.first, pair.second);

		for(const auto & pair : _vector4Uniforms)
			shader->setParameter(pair.first, pair.second);

		for(const auto & pair : _matrix4Uniforms)
			shader->setParameter(pair.first, pair.second);


		uint unitCount = 0;
		for(const auto & pair : _textureUniforms)
		{
			ResourceH<Texture2D> tex = ResourceManager::Get()->getResource<Texture2D>(pair.second);
			shader->setParameter(pair.first, unitCount++, tex->getId());
		}
		
	}
}