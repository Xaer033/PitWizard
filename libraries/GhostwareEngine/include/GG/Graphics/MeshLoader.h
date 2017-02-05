// Ghostware Games inc. 2016  -Julian Williams
#pragma once

#include <GG/Resources/IResourceLoader.h>

namespace GG
{
	class MeshLoader : public IResourceLoader
	{
	public:
		virtual bool	loadDefault(IResource * outResource);

		virtual bool	loadFromFile(IResource * outResource, const std::string & filePath);
		virtual bool	loadFromMemory(IResource * outResource, uint64 size, const void* data);
		virtual bool	loadFromStream(IResource * outResource, IStream * stream);

		virtual bool	loadFromDescriptor(IResource * outResource,
			const IResourceDescriptor * descriptor);

	private:

		bool _isNullResource(IResource * outResource);
	};
}
