#pragma once

#include "IResource.h"
#include "IResourceDescriptor.h"

namespace GG
{
	class IResourceLoader
	{
	public:
		virtual bool	loadDefault(IResource * outResource) = 0;

		virtual bool	loadFromFile(IResource * outResource, const std::string & filePath)		= 0;
		virtual bool	loadFromMemory(IResource * outResource, uint64 size, const void* data)	= 0;
		virtual bool	loadFromStream(IResource * outResource, IStream * stream)				= 0;

		virtual bool	loadFromDescriptor( IResource * outResource, 
											const IResourceDescriptor * descriptor) = 0;
	};
}