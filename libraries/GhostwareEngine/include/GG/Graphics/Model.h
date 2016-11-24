// Ghostware Games inc. 2016  -Julian Williams
#pragma once

#include <GG/Graphics/StaticGeometryBuffer.h>
#include <GG/Resources/IResource.h>
#include <GG/Resources/IResourceDescriptor.h>

#include <vector>


namespace GG
{
	//typedef CIwModel Model;
	typedef StaticGeometryBuffer Model;

	struct MeshDescriptor : public IResourceDescriptor
	{
		MeshDescriptor FromJson(const json & j)
		{
			MeshDescriptor desc;
			return desc;
		}

		MeshDescriptor() : IResourceDescriptor()
		{
		}

	};
	
	struct SubMesh
	{
		int			primitiveType;
		uint		startIndex;
		uint		indexCount;
		StringId	materialId;
		int			materialIndex;

		SubMesh() :
			primitiveType(0),
			startIndex(0),
			indexCount(0),
			materialId(0),
			materialIndex(-1)
		{}
	};

	class Mesh : public IResource
	{
	public:
		DEFINE_RESOURCE_TYPE(Mesh)

	public:
		Mesh() {}
		Mesh(const json & j) {}

		virtual void	init(){}
		virtual void	shutdown(){}

		virtual inline IResourceDescriptor * getDescriptor()
		{
			return &_descriptor;
		}

		

		void addSubMesh(const SubMesh & submesh);

		uint getSubMeshCount() const;
		const SubMesh * getSubMesh(uint index) const;

		StaticGeometryBuffer geoBuffer;
	private:
		
		MeshDescriptor	_descriptor;

		std::vector<SubMesh> _submeshList;
	};
}