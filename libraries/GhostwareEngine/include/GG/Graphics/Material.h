// Ghostware Games inc. 2016  -Julian Williams
#pragma once 

#include <GG/Core/Types.h>
#include <GG/Graphics/RenderState.h>
#include <GG/Graphics/Shader.h>
#include <GG/Graphics/Camera.h>

#include "IwMaterial.h"

namespace GG
{
	typedef CIwMaterial Material;

	struct MaterialBlock
	{
	public:
		MaterialBlock() :
			blendMode(RenderState::BlendMode::BM_NONE),
			cullMode(RenderState::CullMode::CULL_BACK),
			isDepthTesting(true),
			depthRange(Vector2(0.0f, 1.0f)),
			shaderId(-1)
		{
		}
		RenderState::BlendMode	blendMode;
		RenderState::CullMode	cullMode;

		bool		isDepthTesting;
		Vector2		depthRange;

		int32		shaderId;
	};

	class BaseMaterial
	{
	public:
		MaterialBlock	materialBlock;
		
		virtual void	bindMaterialToShader(Shader *	shader) = 0;
	};
}
