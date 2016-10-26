// Ghostware Games inc. 2016  -Julian Williams
#pragma once

#include <GG/Core/StringId.h>
#include <GG/Core/Json.h>

namespace GG
{
	struct IResourceDescriptor
	{
		IResourceDescriptor() :
			resourceId(0),
			type(0)
		{
		}
		// Passed in to load functions to decifer how to load
		// a particular asset
		StringId		resourceId;
		StringId		type;
		std::string		source;
	};
}