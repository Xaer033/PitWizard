#pragma once

#include <GG/Resources/IResourceLoader.h>

namespace GG
{
	class Texture2DLoader : public IResourceLoader
	{
	public:
		virtual bool	loadDefault(IResource * outResource);

		virtual bool	loadFromFile(IResource * outResource, const std::string & fileName);
		virtual bool	loadFromMemory(IResource * outResource, uint64 size, const void* data);
		virtual bool	loadFromStream(IResource * outResource, IStream * stream);

		virtual bool	loadFromDescriptor(IResource * outResource,
			const IResourceDescriptor * descriptor);
	};
}