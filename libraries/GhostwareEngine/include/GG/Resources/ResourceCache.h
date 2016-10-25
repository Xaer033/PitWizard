#pragma once

#include <unordered_map>
#include <GG/Core/StringId.h>
#include "AbstractResourceFactory.h"
#include "ResourceHandle.h"

namespace GG
{
	template<class T>
	using ResourceCache = std::unordered_map<StringId, ResourceHandle<T>>;


	template< class T>
	using DescriptorCache = std::unordered_map<StringId, T>;

	typedef std::unordered_map<StringId, std::unique_ptr<AbstractResourceFactory>>	FactoryMap;

}
