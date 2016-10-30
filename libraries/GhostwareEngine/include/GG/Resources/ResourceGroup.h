// Ghostware Games inc. 2016  -Julian Williams
#pragma once

#include <unordered_map>

#include <GG/Core/IStream.h>
#include <GG/Core/StringId.h>
#include <GG/Core/Json.h>
#include "IResourceDescriptor.h"
#include "ResourceCache.h"

namespace GG
{
	class ResourceGroup
	{
	public:
		static uint32 GetVersion() { return 100; }
		static ResourceGroup FromJson(const json & j);

	public:

		ResourceGroup();
		~ResourceGroup();

		bool	createAllAssets( FactoryMap & factoryMap);
		bool	loadAllAssets();
		void	removeAllAssets();
		void	reloadAllAssets();

		// Use this to create resource that isn't in asset database
		// like a Temporary resource or a placeholder asset
		bool	createResource_DEBUG(const StringId & id);


		const IResourceDescriptor*	getDescriptor(const StringId & id) const;


		inline StringId getGroupId() const
		{
			return _groupId;
		}

	private:
		json	_getJsonFromStream(IStream * stream) const;

		IResourceDescriptor	*	_createDescriptor(
			const json & jsonDesc,
			AbstractResourceFactory * factory);

		std::unordered_map<StringId, IResourceDescriptor*>	_descriptorMap;

		FactoryMap * _factoryMap;

		uint32		_version;
		StringId	_groupId;
		json		_jsonGroup;
	};
}
