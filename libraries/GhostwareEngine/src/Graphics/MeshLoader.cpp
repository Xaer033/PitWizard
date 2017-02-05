
#include "MeshLoader.h"
#include "Model.h"
#include "GmeshLoader.h"

#include <string>
#include <GG/Core/Types.h>
#include <GG/Core/Log.h>

#include <GG/Core/FileStream.h>


namespace GG
{
	using namespace Graphics;

	bool MeshLoader::loadDefault(IResource * outResource)
	{
		if(_isNullResource(outResource)) 
			return false;

		Mesh * mesh = (Mesh*)outResource;
		mesh->geoBuffer.setVertexProperties(VertexProperty::POSITIONS);
		mesh->geoBuffer.addPosition(Vector3(-1,  1,  1));
		mesh->geoBuffer.addPosition(Vector3( 1,  1,  1));
		mesh->geoBuffer.addPosition(Vector3( 1, -1,  1));
		mesh->geoBuffer.addPosition(Vector3( 1, -1,  1));
		mesh->geoBuffer.addPosition(Vector3(-1, -1,  1));
		mesh->geoBuffer.addPosition(Vector3(-1,  1,  1));

		mesh->geoBuffer.addPosition(Vector3(-1,  1, -1));
		mesh->geoBuffer.addPosition(Vector3( 1,  1, -1));
		mesh->geoBuffer.addPosition(Vector3( 1, -1, -1));
		mesh->geoBuffer.addPosition(Vector3( 1, -1, -1));
		mesh->geoBuffer.addPosition(Vector3(-1, -1, -1));
		mesh->geoBuffer.addPosition(Vector3(-1,  1, -1));

		mesh->geoBuffer.addPosition(Vector3( 1,  1, -1));
		mesh->geoBuffer.addPosition(Vector3( 1,  1,  1));
		mesh->geoBuffer.addPosition(Vector3( 1, -1,  1));
		mesh->geoBuffer.addPosition(Vector3( 1, -1,  1));
		mesh->geoBuffer.addPosition(Vector3( 1, -1, -1));
		mesh->geoBuffer.addPosition(Vector3( 1,  1, -1));

		mesh->geoBuffer.addPosition(Vector3(-1,  1, -1));
		mesh->geoBuffer.addPosition(Vector3(-1,  1,  1));
		mesh->geoBuffer.addPosition(Vector3(-1, -1,  1));
		mesh->geoBuffer.addPosition(Vector3(-1, -1,  1));
		mesh->geoBuffer.addPosition(Vector3(-1, -1, -1));
		mesh->geoBuffer.addPosition(Vector3(-1,  1, -1));

		mesh->geoBuffer.addPosition(Vector3( 1,  1, -1));
		mesh->geoBuffer.addPosition(Vector3(-1,  1, -1));
		mesh->geoBuffer.addPosition(Vector3(-1,  1,  1));
		mesh->geoBuffer.addPosition(Vector3(-1,  1,  1));
		mesh->geoBuffer.addPosition(Vector3( 1,  1,  1));
		mesh->geoBuffer.addPosition(Vector3( 1,  1, -1));

		SubMesh submesh;
		submesh.primitiveType = DrawMode::TRIANGLE_LIST;
		submesh.startIndex = 0;
		submesh.indexCount = 24;
		submesh.materialId = -1;
		submesh.materialIndex = 0;
		mesh->addSubMesh(submesh);

		mesh->geoBuffer.build();

		return true;
	}

	bool	MeshLoader::loadFromFile(IResource * outResource, const std::string & filePath)
	{
		if(_isNullResource(outResource))
			return false;

		Mesh * mesh = (Mesh*)outResource;

		FileStream stream(filePath, OpenMode::OPEN_READ);
		if(!stream.isOpen())
		{
			TRACE_WARNING("Could not load model from %s", filePath);
			return false;
		}
		
		return GmeshLoader::LoadFromStream(&stream, mesh);
	}

	bool	MeshLoader::loadFromMemory(IResource * outResource, uint64 size, const void* data)
	{
		return true;
	}

	bool	MeshLoader::loadFromStream(IResource * outResource, IStream * stream)
	{
		if(_isNullResource(outResource))
			return false;

		Mesh * mesh = (Mesh*)outResource;
		return GmeshLoader::LoadFromStream(stream, mesh);
	}
	
	bool	MeshLoader::loadFromDescriptor(IResource * outResource,
				const IResourceDescriptor * descriptor)
	{
		return loadFromFile(outResource, descriptor->source);
	}

	bool	MeshLoader::_isNullResource(IResource * outResource)
	{
		if(outResource == nullptr)
		{
			TRACE_ERROR("Could not load default mesh, mesh is null!");
			return true;
		}
		return false;
	}
}
