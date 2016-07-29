// Ghostware Games inc. 2016  -Julian Williams
#pragma once

#include <GG/Core/Matrix.h>
#include <IwModel.h>

#include "Material.h"


namespace GG
{
	struct RenderCommand3D
	{
		Material *		material;
		CIwModel *		geometry;
		Matrix4	 		modelMatrix;
	};
}