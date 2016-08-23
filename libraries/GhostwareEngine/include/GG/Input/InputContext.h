#pragma once

#include <unordered_map>

#include <GG/Core/Json.h>

#include "EventAction.h"
#include "InputDefines.h"

namespace GG
{
	class InputContext
	{
	public:
		static InputContext FromJson( const json & j );

	public:
		InputContext() {}
		InputContext( const json & j );
		~InputContext();

		inline int getPriority() const { return _priority; }

		void addEventAction( const EventAction & eventAction );

		bool mapEvent( const std::string & gameEvent,  EventAction & eventAction ) const;

	private:

		std::string		_name;
		int32			_priority;


		std::unordered_map< std::string, EventAction > _eventActionMap;
	};
	
	
}