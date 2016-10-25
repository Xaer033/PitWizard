// Ghostware Games inc. 2016  -Julian Williams
#pragma once

#include <string>
#include <unordered_map>

#include <GG/Core/StringId.h>
#include <GG/Core/IStream.h>
#include <GG/Core/FileStream.h>

#include "IResource.h"
#include "ResourceHandle.h"
#include "AbstractResourceFactory.h"
#include "IResourceDescriptor.h"


namespace GG
{
	template<class R>
	class ResourceFactory : public AbstractResourceFactory
	{
	public:
		virtual		~ResourceFactory() {}

		
		virtual ResourceH<R>	getResource(const StringId & id)	final
		{
			auto it = _resourceMap.find(id);
			if(it != _resourceMap.end())
			{
				return it->second;
			}
			// Temp until resource database functionality is added
			std::string resourceName = GetStringFromId(id);
			TRACE_INFO("Adding Texture: %s dynamically", resourceName);
			return addResource(resourceName);
		}

		virtual ResourceH<R>	getResource(const std::string & resourceName)	final
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

		virtual IResourceDescriptor *	createDescriptorFromJson(const json & jsonDescriptor)
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

		virtual bool loadResource(const StringId & id)
		{
			R *  resource = _resourceMap[id].get();
			assert(resource != nullptr);
			IResourceDescriptor * desc = resource->getDescriptor();
			assert(desc != nullptr);

			FileStream stream(desc->source, OpenMode::OPEN_READ);
			resource->loadFromStream(&stream);
			return true;
		}

		virtual bool removeResource(const StringId & id)
		{
			_resourceMap.erase(id);
			return true;
		}

		ResourceH<R>	addResource(const std::string & resourceName)
		{
			assert(resourceName != "null");
			const StringId id = STRING_ID(resourceName);

			auto it = _resourceMap.find(id);
			assert(it == _resourceMap.end());

			R * tex = new R();
			tex->loadFromFile(resourceName); // Temp nullptr descriptor until resource database

			_resourceMap[id] = ResourceHandle<R>(tex);
			return _resourceMap[id];
		}

	private:
		std::unordered_map<StringId, ResourceHandle<R>>	_resourceMap;
	};
}