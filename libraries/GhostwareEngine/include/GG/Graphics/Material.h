// Ghostware Games inc. 2016  -Julian Williams
#pragma once 

#include <GG/Graphics/RenderState.h>
#include "IwMaterial.h"

namespace GG
{
	typedef CIwMaterial Material;

	struct nMaterial
	{
	public:
		BlendMode	blendMode;
		ClearMode	clearMode;
		CullMode	cullMode;

		bool		isDepthTesting;
		Vector2		depthRange;

		uint32		shaderId;

	};
}
