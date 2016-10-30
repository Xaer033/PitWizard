// Ghostware Games inc. 2016  -Julian Williams
#pragma once

#include <iostream>
#include "IResource.h"

namespace GG
{

	template<class T>
	class ResourceHandle
	{
	public:
	
		ResourceHandle() :
			_rawResource(nullptr)
		{
		}

		ResourceHandle(T* resource) : 
			_rawResource(resource)
		{
		}

		inline T* get()
		{
			return _rawResource.get();
		}

		inline T* operator-> ()
		{
			return _rawResource.get();
		}

		void	reload()
		{
			if(_rawResource.get() != nullptr)
				_rawResource.get()->setState(IResource::State::UNLOADED);
		}

	private:
		std::shared_ptr<T>		_rawResource;
	};

	template<class T>
	using ResourceH = ResourceHandle <T> &;
}