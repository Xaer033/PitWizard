#pragma once

#include <GG/Resources/IResourceLoader.h>

namespace GG
{
	class ShaderLoader : public IResourceLoader
	{
	public:
		virtual bool	loadDefault(IResource * outResource);

		virtual bool	loadFromFile(IResource * outResource, const std::string & filePath)	final;
		virtual bool	loadFromMemory(IResource * outResource, uint64 size, const void* data)	final;
		virtual bool	loadFromStream(IResource * outResource, IStream * stream)	final;

		virtual bool	loadFromDescriptor(
			IResource * outResource,
			const IResourceDescriptor * descriptor) final;
	};
}