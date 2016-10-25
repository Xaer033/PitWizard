
// Ghostware Games inc. 2016  -Julian Williams
#pragma once

#include <string>
#include "IResourceDescriptor.h"
#include <GG/Core/IStream.h>


namespace GG
{
	class IResource
	{
	public:
		virtual			~IResource() {}

		virtual bool	loadFromFile(const std::string & filePath)		= 0;
		virtual bool	loadFromMemory(uint32 size, const void* data)	= 0;
		virtual bool	loadFromStream(IStream * stream)				= 0;
		virtual IResourceDescriptor * getDescriptor()					= 0;
	};
}
