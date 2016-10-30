
#include "Texture2DLoader.h"
#include "Texture2D.h"

#include <cassert>
#include <IwImage.h>
#include <s3e.h>

#include <GG/Core/Log.h>
#include <GG/Core/FileStream.h>

namespace GG
{
	bool Texture2DLoader::loadFromFile(IResource * outResource, const std::string & fileName)
	{
		Texture2D * texPtr = (Texture2D*)outResource;
		assert(texPtr != nullptr);

		CIwImage image;
		image.LoadFromFile(fileName.c_str());

		return texPtr->uploadToGPU(image);
	}

	bool Texture2DLoader::loadFromMemory(IResource * outResource, uint64 size, const void* data)
	{
		Texture2D * texPtr = (Texture2D*)outResource;
		assert(texPtr != nullptr);

		CIwImage image;
		s3eFile * f = s3eFileOpenFromMemory((void*)data, (size_t)size);
		image.ReadFile(f);
		s3eFileClose(f);

		return texPtr->uploadToGPU(image);
	}

	bool Texture2DLoader::loadFromStream(IResource * outResource, IStream * stream)
	{
		assert(outResource != nullptr);

		if(stream == nullptr)
		{
			TRACE_WARNING("Stream pointer not valid!");
			return false;
		}

		uint32 size = (uint32)stream->getSize();
		char * buffer = (char*)malloc(size);
		stream->read((void*)buffer, sizeof(char), size);

		bool result = loadFromMemory(outResource, size, buffer);

		free(buffer);
		return result;
	}

	bool Texture2DLoader::loadFromDescriptor(
		IResource * outResource,
		const IResourceDescriptor * descriptor)
	{
		assert(descriptor != nullptr);
		assert(outResource != nullptr);

		FileStream stream(descriptor->source, OpenMode::OPEN_READ);
		if(stream.isOpen())
		{
			return loadFromStream( outResource, &stream);
		}

		return false;
	}
}