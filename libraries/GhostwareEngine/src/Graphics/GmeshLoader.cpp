
#include "GMeshLoader.h"

#include <GG/Core/Types.h>
#include <GG/Core/Log.h>
#include <GG/Core/IStream.h>
#include <GG/Graphics/IVertexBuffer.h>
#include <GG/Graphics/Model.h>

namespace GG
{
	bool GmeshLoader::LoadFromStream(IStream * stream, Mesh * mesh)
	{
		if(stream == nullptr)
		{
			TRACE_ERROR("Could not load mesh, stream is not valid!");
			return false;
		}

		if(mesh == nullptr)
		{
			TRACE_ERROR("Could not load mesh, mesh is not valid!");
			return false;
		}

		int32 sizeOfInt		= sizeof(int32);
		int32 sizeOfUInt	= sizeof(uint);
		int32 sizeOfFloat	= sizeof(float);
		int32 sizeOfVec2	= sizeof(Vector2);
		int32 sizeOfVec3	= sizeof(Vector3);
		int32 sizeOfVec4	= sizeof(Vector4);


		int vertAttribute = 0;
		stream->read(&vertAttribute, sizeOfInt, 1);
		mesh->geoBuffer.setVertexProperties(vertAttribute);


		uint indexSize = 0;
		stream->read(&indexSize, sizeOfUInt, 1);
		for(uint i = 0; i < indexSize; ++i)
		{
			uint index = 0;
			stream->read(&index, sizeOfUInt, 1);
			mesh->geoBuffer.pushIndex(index);
		}

		uint vertSize = 0;
		stream->read(&vertSize, sizeOfUInt, 1);
		for(uint i = 0; i < vertSize; ++i)
		{
			Vector3 pos;
			stream->read(&pos, sizeOfVec3, 1);

			if(vertAttribute & VertexProperty::UV0)
			{
				Vector2 uv0;
				stream->read(&uv0, sizeOfVec2, 1);
				mesh->geoBuffer.pushTexCoord(uv0);
			}

			if(vertAttribute & VertexProperty::UV1)
			{
				Vector2 uv1;
				stream->read(&uv1, sizeOfVec2, 1);
				//mesh->pushTexCoord(uv0); TODO: Implement UV1 channel
			}

			if(vertAttribute & VertexProperty::NORMALS)
			{
				Vector3 normal;
				stream->read(&normal, sizeOfVec3, 1);
				mesh->geoBuffer.pushNormal(normal);
			}

			if(vertAttribute & VertexProperty::TANGENTS)
			{
				Vector3 tangent;
				stream->read(&tangent, sizeOfVec3, 1);
				mesh->geoBuffer.pushTangent(tangent);
			}
			if(vertAttribute & VertexProperty::BITANGENTS)
			{
				Vector3 biTangent;
				stream->read(&biTangent, sizeOfVec3, 1);
				mesh->geoBuffer.pushBitangent(biTangent);
			}
			if(vertAttribute & VertexProperty::COLORS)
			{
				Vector4 color;
				stream->read(&color, sizeOfVec4, 1);
				mesh->geoBuffer.pushColor(color);
			}

			if(vertAttribute & VertexProperty::BONES)
			{
				uint bi_x, bi_y, bi_z, bi_w;

				stream->read(&bi_x, sizeOfUInt, 1);
				stream->read(&bi_y, sizeOfUInt, 1);
				stream->read(&bi_z, sizeOfUInt, 1);
				stream->read(&bi_w, sizeOfUInt, 1);
				Vector4 boneIndex(bi_x, bi_y, bi_z, bi_w);

				Vector4 boneWeight;
				stream->read(&boneWeight, sizeOfVec4, 1);
				//mesh->geoBuffer.pushBoneIndex(boneIndex);
				//mesh->geoBuffer.pushBoneWeight(boneWeight);
			}

			mesh->geoBuffer.addPosition(pos); // Must be added last
		}


		std::string strData;

		int submeshCount = 0;
		stream->read(&submeshCount, sizeOfInt, 1);
		for(int i = 0; i < submeshCount; ++i)
		{
			SubMesh submesh;
			stream->read(&submesh.primitiveType, sizeOfInt, 1);
			stream->read(&submesh.startIndex, sizeOfUInt, 1);
			stream->read(&submesh.indexCount, sizeOfUInt, 1);
			int strSize = 0;
			stream->read(&strSize, sizeOfInt, 1);

			strData.clear();
			strData.resize(strSize);
			stream->read(&strData[0], strSize, 1);
			submesh.materialId = STRING_ID(strData);

			int materialIndex = 0;
			stream->read(&materialIndex, sizeOfInt, 1);
			submesh.materialIndex = materialIndex;
			mesh->addSubMesh(submesh);
		}

		mesh->geoBuffer.build(DrawHint::D_STATIC);
		return true;
	}
}
