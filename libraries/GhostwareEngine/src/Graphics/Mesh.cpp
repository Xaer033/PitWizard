
#include "Model.h"

#include <GG/Core/Log.h>

namespace GG
{
	void Mesh::addSubMesh(const SubMesh & submesh)
	{
		_submeshList.push_back(submesh);
	}

	const SubMesh * Mesh::getSubMesh(uint index) const
	{
		if( index < 0 || index >= _submeshList.size())
		{
			TRACE_WARNING("Index is outside submeshList bounds!");
			return nullptr;
		}

		return &_submeshList[index];
	}

	uint Mesh::getSubMeshCount() const
	{
		return _submeshList.size();
	}
}
