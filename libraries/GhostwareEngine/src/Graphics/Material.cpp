
#include "Material.h"

#include "Shader.h"

namespace GG
{
	void Material::bindToShader(Shader * shader)
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
			shader->setParameter(pair.first, unitCount++, pair.second);
		
	}
}