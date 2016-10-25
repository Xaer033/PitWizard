// Ghostware Games inc. 2016  -Julian Williams
#pragma once

#include <GG/Core/StringId.h>
#include <GG/Core/Json.h>

namespace GG
{
	class AbstractResourceFactory
	{
	public:
		virtual IResourceDescriptor *	createDescriptorFromJson(const json & jsonDescriptor)	= 0;
		virtual bool	loadResource(const StringId & id) = 0;
		virtual bool	removeResource(const StringId & id) = 0;

	};
}