// Ghostware Games inc. 2016  -Julian Williams
#pragma once

#include "RenderableObject.h"
#include "RenderFactory.h"
#include "Material.h"
#include "Model.h"
#include <GG/Core/StringId.h>

#include <GG/EntitySystem/Entity.h>

namespace GG
{
	class MeshInstance : 
		public IRenderableObject, 
		public IComponent
	{
	public:
		DEFINE_COMPONENT_TYPE(MeshInstance)

	public:

		MeshInstance();
		virtual ~MeshInstance();

		virtual void render( RenderFactory & renderFactory );

		void	setMesh(Mesh * pMesh)
		{
			mesh = pMesh;
			if(mesh == nullptr)
			{
				return;
			}

			nMaterialList.clear();
			for(uint i = 0; i < mesh->getSubMeshCount(); ++i)
			{
				nMaterialList.push_back(mesh->getSubMesh(i)->materialId);
			}
		}

		void mainMaterial(const StringId & matId, bool applyToAllMaterials = false)
		{
			if(nMaterialList.empty())
			{
				nMaterialList.push_back(matId);
			}
			else
			{
				uint i = 0;
				do
				{
					nMaterialList[i] = matId;
					++i;
				} while(i < mesh->getSubMeshCount() && applyToAllMaterials);
			}
		}

		StringId mainMaterial() const
		{
			return getMaterialId(0);
		}

		StringId getMaterialId(uint index) const
		{
			return (nMaterialList.empty() || index >= nMaterialList.size()) ? 
					StringId("null") : nMaterialList[index];
		}

		void setMaterialId(uint index, const StringId & matId)
		{
			if(nMaterialList.empty() || index >= nMaterialList.size())
			{
				TRACE_WARNING("Index is outside material list bounderies");
				return;
			}

			nMaterialList[index] = matId;
		}

	private:

		Mesh *		mesh;
		std::vector<StringId> nMaterialList;
	};
}