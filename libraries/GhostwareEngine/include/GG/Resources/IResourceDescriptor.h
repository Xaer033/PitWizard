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

	struct Texture2DDescriptor : public IResourceDescriptor
	{
		enum WrapMode
		{
			WM_REPEAT, 
			WM_CLAMP
		};

		static Texture2DDescriptor FromJson(const json & j)
		{
			Texture2DDescriptor desc;
			desc.resourceId		= STRING_ID(j.get("id", Json::Value("null")).asString());
			desc.type			= STRING_ID(j.get("type", Json::Value("null")).asString());
			desc.source			= j.get("source", Json::Value("null")).asString();
			desc.isMipmapped	= j.get("mipMapping", Json::Value(true)).asBool();
			desc.wrapMode		= (j.get("wrapMode", Json::Value("repeat")).asString() == "repeat") ? WrapMode::WM_REPEAT : WrapMode::WM_CLAMP;
			
			return desc;
		}

		Texture2DDescriptor() : 
			IResourceDescriptor(),
			wrapMode(WM_REPEAT), 
			isMipmapped(true)
		{

		}

		int16	wrapMode;
		bool	isMipmapped;

	};
}