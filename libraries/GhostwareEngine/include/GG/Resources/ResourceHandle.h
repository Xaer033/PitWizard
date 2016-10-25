// Ghostware Games inc. 2016  -Julian Williams
#pragma once

#include <iostream>

namespace GG
{

	template<class T>
	class ResourceHandle
	{
	public:
		enum class State
		{
			UNKNOWN = 0,
			UNLOADED,
			LOADED
		};

		ResourceHandle() :
			_state(State::UNKNOWN),
			_rawResource(nullptr)
		{
		}

		ResourceHandle(T* resource) :
			_state(State::UNKNOWN),
			_rawResource(resource)
		{
			
		}


		inline State	getState() const
		{
			return _state;
		}


		inline const T*	get() const
		{
			return _rawResource.get();
		}

		inline T*	get()
		{
			return _rawResource.get();
		}

		void	reload()
		{

		}

	private:
		State					_state;
		std::shared_ptr<T>		_rawResource;
	};

	template<class T>
	using ResourceH = ResourceHandle <T> &;
}