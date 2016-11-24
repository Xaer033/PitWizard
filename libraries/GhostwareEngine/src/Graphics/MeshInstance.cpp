
#include "MeshInstance.h"

#include "Material.h"
#include "Model.h"
#include <GG/Core/Log.h>


namespace GG
{
	MeshInstance::MeshInstance() :
		mesh(nullptr)
	{
	}

	MeshInstance::~MeshInstance()
	{
		mesh = nullptr;
		nMaterialList.clear();
	}

	void MeshInstance::render( RenderFactory & renderFactory )
	{
		const GG::Matrix4 & worldMat = _entity->getSceneNode()->modelToWorldMatrix();
		if(mesh == nullptr)
		{
			return;
		}

		for(uint i = 0; i < mesh->getSubMeshCount(); ++i)
		{
			const SubMesh & submesh = *mesh->getSubMesh(i);

			renderFactory.addCommand(
				&mesh->geoBuffer,
				nMaterialList[i],
				submesh,
				worldMat
			);
		}
	}
}
