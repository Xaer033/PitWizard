// Ghostware Games inc. 2016  -Julian Williams
#pragma once

#include <GG/Resources/IResourceDescriptor.h>
#include <IwGL.h>

namespace GG
{
	struct Texture2DDescriptor : public IResourceDescriptor
	{
		static Texture2DDescriptor FromJson(const json & j);
		Texture2DDescriptor();

		int16	wrapMode;
		int16	minFilterMode;
		int16	magFilterMode;

		bool	isMipmapped;

	};
}
