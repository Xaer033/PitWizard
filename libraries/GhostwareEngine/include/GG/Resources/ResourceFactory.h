// Ghostware Games inc. 2016  -Julian Williams
#pragma once

#include <string>
#include <cassert>
#include <unordered_map>
#include <set>

#include <GG/Core/StringId.h>
#include <GG/Core/IStream.h>
#include <GG/Core/FileStream.h>

#include "IResource.h"
#include "ResourceCache.h"
#include "ResourceHandle.h"
#include "AbstractResourceFactory.h"
#include "IResourceDescriptor.h"
#include "IResourceLoader.h"


namespace GG
{
	template<class R>
	class ResourceFactory : public AbstractResourceFactory
	{
	public:
		ResourceFactory(IResourceLoader * loader);
		virtual		~ResourceFactory() {}
		
		ResourceH<R>	getResource(const StringId & id);
		ResourceH<R>	getResource(const std::string & resourceName);
		ResourceH<R>	addResource(const std::string & resourceName);

		virtual IResourceDescriptor *	createDescriptorFromJson(const json & jsonDescriptor);

		virtual bool	loadResource(const StringId & id);
		virtual bool	removeResource(const StringId & id);

	private:
		static ResourceHandle<R>			_defaultResource;
		ResourceCache<R>					_resourceMap;
		std::unique_ptr<IResourceLoader>	_resourceLoader;
		std::set<StringId>					_errorCacheIds;
	};



	template<class R>
	ResourceHandle<R> ResourceFactory<R>::_defaultResource;


	template<class R>
	ResourceFactory<R>::ResourceFactory(IResourceLoader * loader) :
		_resourceLoader(loader)
	{
		assert(_resourceLoader != nullptr);
		_defaultResource = ResourceHandle<R>(new R());
		R* resource = _defaultResource.get();
		assert(_resourceLoader->loadDefault(resource));
		resource->setState(IResource::State::LOADED);
	}

	template<class R>
	ResourceH<R> ResourceFactory<R>::getResource(const StringId & id)
	{
		auto it = _resourceMap.find(id);
		if(it == _resourceMap.end())
		{
			if(_errorCacheIds.find(id) == _errorCacheIds.end())
			{
				TRACE_WARNING("Resource Loader could not find asset: %s", GetStringFromId(id));
				_errorCacheIds.insert(id);
			}
			return _defaultResource;
		}

		/*if(it->second->getState() == IResource::State::UNLOADED)
		{
			it->second->shutdown();
			if(!loadResource(it->first))
			{
				return _defaultResource;
			}
		}*/
		return it->second;
	}

	template<class R>
	ResourceH<R> ResourceFactory<R>::getResource(const std::string & resourceName)
	{
		const StringId id = STRING_ID(resourceName);

		auto it = _resourceMap.find(id);
		if(it != _resourceMap.end())
		{
			return it->second;
		}

		// Temp until resource database functionality is added

		TRACE_INFO("Adding Texture: %s dynamically", resourceName);
		return addResource(resourceName);
	}

	template<class R>
	IResourceDescriptor *	ResourceFactory<R>::createDescriptorFromJson(const json & jsonDescriptor)
	{
		R * rawResource = new R(jsonDescriptor);
		IResourceDescriptor * descriptor = rawResource->getDescriptor();
		assert(descriptor != nullptr);

		const StringId id = descriptor->resourceId;

		// Place default asset if one hasn't been loaded yet (or can't be found)
		// useful for placeholder assets
		_resourceMap[id] = ResourceHandle<R>(rawResource);

		return descriptor;
	}

	template<class R>
	bool ResourceFactory<R>::loadResource(const StringId & id)
	{
		assert(_resourceLoader.get() != nullptr);

		R *  resource = _resourceMap[id].get();
		if(resource == nullptr)
		{
			TRACE_WARNING("Resource pointer for: %s is invalid", GetStringFromId(id));
			return false;
		}

		IResourceDescriptor * desc = resource->getDescriptor();
		assert(desc != nullptr);

		if(resource->getState() == IResource::State::LOADED)
		{
			resource->shutdown();
		}

		bool result = _resourceLoader->loadFromDescriptor(resource, desc);

		if(result)
		{
			resource->setState(IResource::State::LOADED);
			resource->init();
		}
		return result;
	}

	template<class R>
	bool ResourceFactory<R>::removeResource(const StringId & id)
	{
		_resourceMap[id]->shutdown();
		_resourceMap.erase(id);
		return true;
	}

	template<class R>
	ResourceH<R> ResourceFactory<R>::addResource(const std::string & resourceName)
	{
		assert(resourceName != "null");
		const StringId id = STRING_ID(resourceName);

		auto it = _resourceMap.find(id);
		assert(it == _resourceMap.end());

		R * tex = new R();
		_resourceLoader->loadFromFile(tex, resourceName);
		_resourceMap[id] = ResourceHandle<R>(tex);
		return _resourceMap[id];
	}
}