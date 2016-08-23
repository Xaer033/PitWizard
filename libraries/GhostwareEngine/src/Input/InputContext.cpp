
#include "InputContext.h"

#include <algorithm>
#include <unordered_map>

#include <GG/Core/Vector.h>
#include <GG/Core/Json.h>
#include <GG/Core/Log.h>

#include "InputDefines.h"
#include "EventAction.h"

namespace GG
{
	
	InputContext InputContext::FromJson( const json & j )
	{
		return InputContext( j );
	}

	InputContext::InputContext( const json & j )
	{
		_name = j.get( "context", json::nullRef ).asString();
		_priority = j.get( "priority", json::nullRef ).asInt();

		auto inputList = j.get( "input_list", json::nullRef );
		if( inputList.isNull() )
		{
			TRACE_WARNING( "Input Context action list is not found." );
			return;
		}

		for( uint i = 0; i < inputList.size(); ++i )
		{
			addEventAction( EventAction::FromJson( inputList[ i ] ) );
		}
	}

	InputContext::~InputContext()
	{
		_eventActionMap.clear();
	}

	void InputContext::addEventAction( const EventAction & eventAction )
	{
		_eventActionMap[ eventAction.name ] = eventAction;
	}

	bool InputContext::mapEvent( const std::string & gameEvent, EventAction & eventAction ) const
	{
		auto it = _eventActionMap.find( gameEvent );
		if( it == _eventActionMap.end() )
			return false;
		
		eventAction = it->second;
		return true;
	}
}