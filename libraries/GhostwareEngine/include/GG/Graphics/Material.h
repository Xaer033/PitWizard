// Ghostware Games inc. 2016  -Julian Williams
#pragma once 
#include <unordered_map>

#include <GG/Core/Types.h>
#include <GG/Core/Vector.h>
#include <GG/Graphics/RenderState.h>
#include <GG/Graphics/Shader.h>
#include <GG/Graphics/Camera.h>

namespace GG
{
	struct RenderStateBlock
	{
	public:
		RenderStateBlock() :
			blendMode(RenderState::BlendMode::BM_NONE),
			cullMode(RenderState::CullMode::CULL_BACK),
			windingMode(RenderState::WindingMode::WM_COUNTER_CLOCKWISE),
			isDepthTesting(true),
			depthRange(Vector2(0.0f, 1.0f)),
			shaderId(-1)
		{
		}
		RenderState::BlendMode		blendMode;
		RenderState::CullMode		cullMode;
		RenderState::WindingMode	windingMode;

		bool		isDepthTesting;
		Vector2		depthRange;

		int32		shaderId;
	};

	class Material
	{
		friend class MaterialSerializer;

	public:
		Material()
		{
		}

		RenderStateBlock	renderStateBlock;
		
		void bindToShader(Shader *	shader);

	private:

		std::unordered_map<std::string, int>		_intUniforms;
		std::unordered_map<std::string, float>		_floatUniforms;
		std::unordered_map<std::string, Vector2>	_vector2Uniforms;
		std::unordered_map<std::string, Vector3>	_vector3Uniforms;
		std::unordered_map<std::string, Vector4>	_vector4Uniforms;
		std::unordered_map<std::string, Matrix4>	_matrix4Uniforms;
		std::unordered_map<std::string, uint>		_textureUniforms;

	};
}
