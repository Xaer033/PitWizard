// Ghostware Games inc. 2016  -Julian Williams
#pragma once

#include <GG/Core/Matrix.h>
#include "Model.h"
#include "Material.h"


namespace GG
{
	struct RenderCommand3D
	{
		Material *		material;
		Model *			geometry;
		Matrix4	 		modelMatrix;
	};

	struct RenderCommandSubMesh
	{
		IVertexBuffer * vertexBuffer;
		StringId		materialId;
		SubMesh			subMesh;
		Matrix4	 		modelMatrix;
	};
}