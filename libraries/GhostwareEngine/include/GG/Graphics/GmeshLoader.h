// Ghostware Games inc. 2016  -Julian Williams
#pragma once

namespace GG
{
	class IStream;
	class Mesh;

	class GmeshLoader
	{
	public:
		static bool LoadFromStream(IStream * stream, Mesh * mesh);
		//bool loadFromStream(IStream * stream, Mesh * mesh);
	};
}
