
#include "Texture2DDescriptor.h"

#include <algorithm>
#include <GG/Core/Log.h>
#include <GG/Core/Json.h>
#include "Texture2D.h"

namespace GG
{
	TextureWrapMode GetWrapModeFromString(const std::string & wrapModeStr)
	{
		std::string key = wrapModeStr;
		std::transform(key.begin(), key.end(), key.begin(), ::tolower);

		if(key == "repeat")					{ return TextureWrapMode::WM_REPEAT; }
		else if(key == "mirrored_repeat")	{ return TextureWrapMode::WM_MIRRORED_REPEAT; }
		else if(key == "clamp")				{ return TextureWrapMode::WM_CLAMP; }

		TRACE_WARNING("Could not find wrap mode: %s", key);
		return TextureWrapMode::WM_CLAMP;
	}

	TextureFilterMode GetFilterModeFromString(const std::string & filterModeStr)
	{
		std::string key = filterModeStr;
		std::transform(key.begin(), key.end(), key.begin(), ::tolower);

		if(key == "nearest")				{ return TextureFilterMode::FM_NEAREST; }
		else if(key == "linear")			{ return TextureFilterMode::FM_LINEAR; }
		else if(key == "mipmap_nearest")		{ return TextureFilterMode::FM_MIPMAP_NEAREST; }
		else if(key == "mipmap_linear")		{ return TextureFilterMode::FM_MIPMAP_LINEAR; }

		TRACE_WARNING("Could not find filter mode: %s", key);
		return TextureFilterMode::FM_LINEAR;
	}

	Texture2DDescriptor Texture2DDescriptor::FromJson(const json & j)
	{
		Texture2DDescriptor desc;
		desc.resourceId		= STRING_ID(j.get("id", Json::Value("null")).asString());
		desc.type			= STRING_ID(j.get("type", Json::Value("null")).asString());
		desc.source			= j.get("source", Json::Value("null")).asString();
		desc.isMipmapped	= j.get("mipMapping", Json::Value(true)).asBool();
		desc.wrapMode		= GetWrapModeFromString(j.get("wrapMode", Json::Value("repeat")).asString());
		desc.minFilterMode	= GetFilterModeFromString(j.get("minFilterMode", Json::Value("mipmapLinear")).asString());
		desc.magFilterMode	= GetFilterModeFromString(j.get("magFilterMode", Json::Value("linear")).asString());

		return desc;
	}

	Texture2DDescriptor::Texture2DDescriptor() :
		IResourceDescriptor(),
		wrapMode(WM_REPEAT),
		minFilterMode(FM_MIPMAP_LINEAR),
		magFilterMode(FM_LINEAR),
		isMipmapped(true)
	{
	}

}