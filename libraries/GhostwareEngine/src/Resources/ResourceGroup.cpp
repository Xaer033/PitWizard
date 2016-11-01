
#include "ResourceGroup.h"

#include <GG/Core/StringId.h>
#include <GG/Core/Log.h>
#include <GG/Core/FileSystem.h>
#include <GG/Graphics/Texture2D.h>
#include <cassert>
#include <Physfs/physfs.h>

namespace GG
{
	ResourceGroup ResourceGroup::FromJson(const json & j)
	{
		ResourceGroup group;
		group._jsonGroup	= j;
		group._version		= j.get("version", Json::Value(GetVersion())).asInt();
		group._groupId		= STRING_ID(j.get("group", Json::Value("poop")).asString());

		Json::Value paths	= j.get("paths", Json::arrayValue);
		for(uint i = 0; i < paths.size(); ++i)
		{
			std::string path = paths[i].asString();
			bool result = FileSystem::Mount(path, "/", false);
			if(!result)
			{
				TRACE_WARNING("Error adding path: %s", FileSystem::GetLastError());
			}
		}
		return group;
	}

	ResourceGroup::ResourceGroup() :
		_groupId(0),
		_version(100),
		_factoryMap(nullptr)
	{
	}

	ResourceGroup::~ResourceGroup()
	{
		_descriptorMap.clear();
	}

	bool ResourceGroup::createAllAssets(FactoryMap & factoryMap)
	{
		_factoryMap = &factoryMap;

		json resources = _jsonGroup.get("resources", Json::arrayValue);
		for(uint i = 0; i < resources.size(); ++i)
		{
			json		jsonDesc		= resources[i];
			StringId	resourceType	= STRING_ID(jsonDesc["type"].asString());

			AbstractResourceFactory *	factory = factoryMap[resourceType].get();
			IResourceDescriptor *		desc	= _createDescriptor(jsonDesc, factory);
			
			TRACE_INFO("Created Description for asset: %s", GetStringFromId(desc->resourceId));
			
			if(desc == nullptr)
			{
				TRACE_ERROR("Resource of type: %s couldn't be created!", GetStringFromId(resourceType));
				return false;
			}
		}

		return true;
	}

	bool ResourceGroup::loadAllAssets()
	{
		assert(_factoryMap != nullptr);

		auto it = _descriptorMap.begin();
		for(; it != _descriptorMap.end(); ++it)
		{
			const StringId resourceId			= it->first;
			const IResourceDescriptor * desc	= it->second;
			const StringId resourceType			= desc->type;
			assert(desc != nullptr);


			std::string resourceName = GetStringFromId(resourceId);
			TRACE_DEBUG("Loading Resource: %s", resourceName);

			bool result = (*_factoryMap)[resourceType]->loadResource(resourceId);
			if(!result)
			{
				TRACE_WARNING("Error trying to load resource: %", resourceName);
			}
		}

		return true;
	}

	void ResourceGroup::removeAllAssets()
	{
		assert(_factoryMap != nullptr);

		auto it = _descriptorMap.begin();
		for(; it != _descriptorMap.end(); ++it)
		{
			const StringId resourceId			= it->first;
			const IResourceDescriptor * desc	= it->second;
			const StringId resourceType			= desc->type;

			(*_factoryMap)[resourceType]->removeResource(resourceId);

		}
		_descriptorMap.clear();
	}

	void ResourceGroup::reloadAllAssets()
	{
		auto it = _descriptorMap.begin();
		for(; it != _descriptorMap.end(); ++it)
		{
			const StringId resourceId			= it->first;
			const IResourceDescriptor * desc	= it->second;
			const StringId resourceType			= desc->type;
			
			(*_factoryMap)[resourceType]->loadResource(resourceId);
		}
		/*removeAllAssets();
		createAllAssets(*_factoryMap);
		loadAllAssets();*/
	}

	const IResourceDescriptor * ResourceGroup::getDescriptor(const StringId & id) const
	{
		auto it = _descriptorMap.find(id);
		if(it == _descriptorMap.end())
		{
			TRACE_ERROR("Unable to find Description for id: %d", id);
			return nullptr;
		}
		return it->second;
	}

// -------------------------------------------------------------------------------------

	json ResourceGroup::_getJsonFromStream(IStream * stream) const
	{
		if(stream == nullptr)
		{
			TRACE_ERROR("Resource Group cannot read from invalid stream!");
			assert(0);
		}
		
		uint32 size = (uint32)stream->getSize();
		char * buffer = (char*)malloc(size);
		stream->read(buffer, sizeof(char), size);
		std::string jsonStr(buffer);
		free(buffer);

		return JsonFromString(jsonStr);
	}


	IResourceDescriptor	* ResourceGroup::_createDescriptor(
		const json & jsonDesc, 
		AbstractResourceFactory * factory)
	{
		assert(factory != nullptr);
		IResourceDescriptor * desc = factory->createDescriptorFromJson(jsonDesc);
		assert(desc != nullptr);

		_descriptorMap[desc->resourceId] = desc;
		return desc;
	}	
}