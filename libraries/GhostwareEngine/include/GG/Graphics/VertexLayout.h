// Ghostware Games inc. 2016  -Julian Williams
#pragma once

#include <GG/Core/Types.h>
#include <string>

namespace GG
{
	namespace Graphics
	{
		enum VertexProperty
		{
			POSITIONS		= 1 << 0,
			UV0				= 1 << 1,
			UV1				= 1 << 2,
			NORMALS			= 1 << 3,
			TANGENTS		= 1 << 4,
			BITANGENTS		= 1 << 5,
			COLORS			= 1 << 6,
			BONE_INDICIES	= 1 << 7,
			BONE_WEIGHTS	= 1 << 8,
			USER_0			= 1 << 9,
			USER_1			= 1 << 10
		};
		class VertexAttribute
		{
		public:
			static int32 GetIndex(const VertexProperty & attrib);
			static VertexProperty FromString(const std::string & attribStr);

			static int32 GetIndexFromString(const std::string & attribStr);
		};
	}
}