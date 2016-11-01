// Ghostware Games inc. 2016  -Julian Williams
#pragma once

#include <GG/Resources/IResourceDescriptor.h>

namespace GG
{
	struct MaterialDescriptor : public IResourceDescriptor
	{
		static MaterialDescriptor FromJson(const json & j)
		{
			MaterialDescriptor desc;
			desc.resourceId		= STRING_ID(j.get("id", Json::Value("null")).asString());
			desc.type			= STRING_ID(j.get("type", Json::Value("null")).asString()); 
			desc.source			= j.get("source", Json::Value("null")).asString();
			return desc;
		}

		MaterialDescriptor() :
			IResourceDescriptor()
		{
		}
	};
}
