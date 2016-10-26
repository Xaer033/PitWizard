// Ghostware Games inc. 2016  -Julian Williams
#pragma once

#include <unordered_map>
#include <GG/Core/StringId.h>
#include <GG/Core/Log.h>
#include <GG/Core/IStream.h>

#include "IResource.h"
#include "ResourceFactory.h"
#include "ResourceHandle.h"
#include "ResourceCache.h"
#include "ResourceGroup.h"
#include <cassert>


namespace GG
{
	class ResourceManager
	{
	public:

		static ResourceManager * GetInstance()
		{
			if(_instance == nullptr)
				_instance = new ResourceManager();

			return _instance;
		}

		static void	ShutDown()
		{
			if(_instance != nullptr)
			{
				_instance->_factoryMap.clear();
				delete _instance;
			}
		}

	public:

		template<class T>
		const ResourceH<T>	getResource(const StringId & id) const
		{
			return _getResource<T>(id);
		}

		template<class T>
		ResourceH<T>	getResource(const StringId & id)
		{
			return _getResource<T>(id);
		}

		template<class T>
		ResourceH<T>	getResource(const std::string & resourceName)
		{
			return _getResource<T>(resourceName);
		}

		template<class T>
		void	registerType()
		{
			const StringId resourceType = T::GetResourceType();
			auto it = _factoryMap.find(resourceType);
			if(it != _factoryMap.end())
			{
				TRACE_ERROR("Already registered type: %s", GetStringFromId(resourceType));
				return;
			}
			
			_factoryMap[resourceType] = std::unique_ptr<ResourceFactory<T>>(new ResourceFactory<T>());
		}


		ResourceGroup * createGroup( IStream * stream )
		{
			assert(stream != nullptr);

			uint32 size = (uint32)stream->getSize();
			char * buffer = (char*)malloc(size);
			stream->read(buffer, sizeof(char), size);
			std::string jsonStr(buffer);
			free(buffer);

			json j = JsonFromString(jsonStr);

			ResourceGroup group = ResourceGroup::FromJson(j);
			StringId groupId	= group.getGroupId();

			_groupMap[groupId]	= group;
			_groupMap[groupId].createAllAssets(_factoryMap);

			return &_groupMap[groupId];
		}

		ResourceGroup * findGroup(const StringId & id)
		{
			auto it = _groupMap.find(id);
			if(it == _groupMap.end())
			{
				TRACE_ERROR("Could not find asset group: %s in storage!", GetStringFromId(id));
				return nullptr;
			}
			return &it->second;
		}

	private:
		static ResourceManager *	_instance;

		ResourceManager() {}

	private:
		template<class T>
		const ResourceH<T> _getResource(const StringId & id) const
		{
			const StringId resourceType = T::GetResourceType();
			ResourceFactory<T> * resFactory = (ResourceFactory<T>*)_getFactory(resourceType);
			assert(resFactory != nullptr);
			return resFactory->getResource(id);
		}

		template<class T>
		ResourceH<T> _getResource(const StringId & id)
		{
			const StringId resourceType = T::GetResourceType();
			ResourceFactory<T> * resFactory =  (ResourceFactory<T>*)_getFactory(resourceType);
			assert(resFactory != nullptr);
			return resFactory->getResource(id);
		}

		template<class T>
		ResourceH<T> _getResource(const std::string & resourceName)
		{
			const StringId resourceType = T::GetResourceType();
			ResourceFactory<T> * resFactory =  (ResourceFactory<T>*)_getFactory(resourceType);
			assert(resFactory != nullptr);
			return resFactory->getResource(resourceName);
		}

		template<class T>
		ResourceH<T> _getResource(IStream * stream)
		{
			const StringId resourceType = T::GetResourceType();
			ResourceFactory<T> * resFactory =  (ResourceFactory<T>*)_getFactory(resourceType);
			assert(resFactory != nullptr);
			return resFactory->getResource(stream);
		}



		AbstractResourceFactory* _getFactory(const StringId & resourceType) const
		{
			auto it = _factoryMap.find(resourceType);
			if(it == _factoryMap.end())
			{
				TRACE_ERROR("Can't find Resource Factory of type: %s", GetStringFromId(resourceType));
				return nullptr;
			}
			return it->second.get();
		}


	private:
		std::unordered_map<StringId, ResourceGroup> _groupMap;

		FactoryMap		_factoryMap;
		
	};
}