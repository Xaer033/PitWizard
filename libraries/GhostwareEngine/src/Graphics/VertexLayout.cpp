#include "VertexLayout.h"
#include <GG/Core/Log.h>

namespace GG
{
	namespace Graphics
	{
		int32 VertexAttribute::GetIndex(const VertexProperty & attrib)
		{
			switch((int)attrib)
			{
			case VertexProperty::POSITIONS:		return 0;
			case VertexProperty::UV0:			return 1;
			case VertexProperty::UV1:			return 2;
			case VertexProperty::NORMALS:		return 3;
			case VertexProperty::TANGENTS:		return 4;
			case VertexProperty::BITANGENTS:	return 5;
			case VertexProperty::COLORS:		return 6;
			case VertexProperty::BONE_INDICIES:	return 7;
			case VertexProperty::BONE_WEIGHTS:	return 8;
			case VertexProperty::USER_0:		return 9;
			case VertexProperty::USER_1:		return 10;
			}

			TRACE_WARNING("Could not get index for attribute type: %d", attrib);
			return -1;
		}
		
		VertexProperty VertexAttribute::FromString(const std::string & attribStr)
		{
			std::string key = attribStr;
			std::transform(key.begin(), key.end(), key.begin(), ::tolower);

			if(key == "position") { return VertexProperty::POSITIONS; }
			else if(key == "uv0") { return VertexProperty::UV0; }
			else if(key == "uv1") { return VertexProperty::UV1; }
			else if(key == "normal") { return VertexProperty::NORMALS; }
			else if(key == "tangent") { return VertexProperty::TANGENTS; }
			else if(key == "bitangent") { return VertexProperty::BITANGENTS; }
			else if(key == "color") { return VertexProperty::COLORS; }
			else if(key == "bone_indice") { return VertexProperty::BONE_INDICIES; }
			else if(key == "bone_weight") { return VertexProperty::BONE_WEIGHTS; }

			TRACE_WARNING("Vertex Attribute Key not supported: %s", key);
			return (VertexProperty)-1;
		}

		int32 VertexAttribute::GetIndexFromString(const std::string & attribStr)
		{
			const VertexProperty attrib = FromString(attribStr);
			return GetIndex(attrib);
		}
	}
}