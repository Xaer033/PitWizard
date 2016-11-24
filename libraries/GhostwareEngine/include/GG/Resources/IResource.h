
// Ghostware Games inc. 2016  -Julian Williams
#pragma once

#include <GG/Core/StringId.h>
#include <GG/Core/IStream.h>
#include <GG/Core/Json.h>
#include "IResourceDescriptor.h"


namespace GG
{

#define DEFINE_RESOURCE_TYPE(x) \
	static StringId GetResourceType()\
	{\
		static StringId id = STRING_ID(#x);\
		return id;\
	}


	class IResource
	{
	public:
		enum class State
		{
			UNKNOWN = 0,
			UNLOADED,
			LOADED
		};

		IResource() : _state(State::UNKNOWN) {}
		IResource(const json & j) : _state(State::UNKNOWN) {}

		virtual			~IResource() {}

		virtual void					init()			= 0;
		virtual	void					shutdown()		= 0;

		virtual IResourceDescriptor *	getDescriptor()	= 0;
		
		

		inline State	getState() const
		{
			return _state;
		}

		inline void		setState(State resourceState)
		{
			_state = resourceState;
		}

	protected:
		State	_state;
	};
}
