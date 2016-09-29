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
			blendMode(BlendMode::BM_NONE),
			cullMode(CullMode::CULL_BACK),
			isDepthTesting(true),
			depthRange(Vector2(0.0f, 1.0f)),
			shaderId(-1)
		{
		}
		BlendMode	blendMode;
		CullMode	cullMode;

		bool		isDepthTesting;
		Vector2		depthRange;

		int32		shaderId;
	};

	class BaseMaterial
	{
	public:
		MaterialBlock	materialBlock;
		
		virtual void	bindMaterialToShader(
			const RenderState & renderState,
			const Matrix4 & worldMatrix,
			const Camera *	camera,
			const Shader *	shader) const = 0;
	};
}
