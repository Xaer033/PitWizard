
#include "MaterialLoader.h"
#include "Material.h"
#include "MaterialSerializer.h"

#include <cassert>
#include <GG/Core/Log.h>
#include <GG/Core/FileStream.h>
#include <GG/Core/StreamHelper.h>


namespace GG
{
	bool MaterialLoader::loadDefault(IResource * outResource)
	{
		static const std::string kDefaultMaterialStr = 
			"{\n"
			"	\"materialValues\" : [\n"
			"	],\n"
			"	\"renderStateBlock\" : {\n"
			"		\"blendMode\" 	: \"none\",\n"
			"			\"cullMode\" : \"back\",\n"
			"			\"windingMode\" : \"ccw\",\n"
			"			\"depthRange\" : [0, 1],\n"
			"			\"depthTest\" : true,\n"
			"			\"shaderId\" : \"null\"\n"
			"	},\n"
			"	\"version\" : 1\n"
			"}\n";

		Material* mat = (Material*)outResource;
		assert(mat != nullptr);
		return MaterialSerializer::Deserialize(*mat, kDefaultMaterialStr);
	}

	bool MaterialLoader::loadFromFile(IResource * outResource, const std::string & fileName)
	{
		Material * mat = (Material*)outResource;
		assert(mat != nullptr);

		FileStream stream(fileName, OpenMode::OPEN_READ);

		return MaterialSerializer::Deserialize(*mat, Stream::ReadStringFromStream(&stream));
	}

	bool MaterialLoader::loadFromMemory(IResource * outResource, uint64 size, const void* data)
	{
		Material * mat = (Material*)outResource;
		assert(mat != nullptr);
		
		return MaterialSerializer::Deserialize(*mat, std::string((char*)data, (uint32)size));
		return true;
	}

	bool MaterialLoader::loadFromStream(IResource * outResource, IStream * stream)
	{
		Material * mat = (Material*)outResource;
		assert(mat != nullptr);

		return MaterialSerializer::Deserialize(*mat, Stream::ReadStringFromStream(stream));
	}

	bool MaterialLoader::loadFromDescriptor(
		IResource * outResource,
		const IResourceDescriptor * descriptor)
	{
		Material * mat = (Material*)outResource;
		assert(mat != nullptr);

		FileStream stream(descriptor->source, OpenMode::OPEN_READ);
		return MaterialSerializer::Deserialize(*mat, Stream::ReadStringFromStream(&stream));
	}
}