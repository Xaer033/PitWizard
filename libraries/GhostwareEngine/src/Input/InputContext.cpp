
#include "InputContext.h"

#include <unordered_map>
#include <s3eKeyboard.h>
#include <s3ePointer.h>

#include <GG/Core/Json.h>
#include <GG/Core/Log.h>

#include "RawInput.h"

namespace GG
{
	InputType EventAction::_GetTypeFromString( const std::string & typeStr )
	{
		if( typeStr == "key_button" )			{ return InputType::KEY_BUTTON;	}
		else if( typeStr == "pointer_button" )	{ return InputType::POINTER_BUTTON; }
		else if( typeStr == "touch" )			{ return InputType::TOUCH;	}
		else if( typeStr == "axis" )			{ return InputType::AXIS; }

		if( !typeStr.empty() )
		{
			TRACE_WARNING( "Input Type: %s not supported!", typeStr );
		}

		return InputType::NONE;
	}

	AxisType EventAction::_GetAxisTypeFromString( const std::string & axisType )
	{
		if( axisType == "mouse_x" )				{ return AxisType::MOUSE_X; }
		else if( axisType == "mouse_y" )		{ return AxisType::MOUSE_Y; }
		else if( axisType == "key_button" )		{ return AxisType::KEY_BUTTON; }
		else if( axisType == "pointer_putton" )	{ return AxisType::POINTER_BUTTON; }
		else if( axisType == "touch_drag_x" )	{ return AxisType::TOUCH_DRAG_X; }
		else if( axisType == "touch_drag_y" )	{ return AxisType::TOUCH_DRAG_Y; }

		if( !axisType.empty() )
		{
			TRACE_WARNING( "Axis Type: %s not supported!", axisType );
		}

		return AxisType::NONE;
	}

	InputMode EventAction::_GetModeFromString( const std::string & modeStr )
	{
		if( modeStr == "up" )				{ return InputMode::UP;			}
		else if( modeStr == "down" )		{ return InputMode::DOWN;		}
		else if( modeStr == "pressed" )		{ return InputMode::PRESSED;	}
		else if( modeStr == "released" )	{ return InputMode::RELEASED;	}

		return InputMode::UP;
	}

	EventAction::EventAction( const json & j )
	{
		name	= j.get( "name", Json::nullValue ).asString();
		TRACE_DEBUG( "EventAction Name: %s", name );


		auto primValue				= j.get( "primary", json::nullRef );
		primary.type				= _GetTypeFromString( primValue.get( "type", "" ).asString() );
		primary.button				= primValue.get( "button", -1 ).asInt();
		primary.negativeButton		= primValue.get( "negative_button", -1 ).asInt();
		primary.mode				= _GetModeFromString( primValue.get( "mode", "down").asString() );
		primary.axisType			= _GetAxisTypeFromString( primValue.get( "axis_type", "").asString() );
		primary.min					= primValue.get( "min", 0 ).asFloat();
		primary.max					= primValue.get( "max", 0 ).asFloat();

		auto secValue				= j.get( "secondary", json::nullRef );
		secondary.type				= _GetTypeFromString( secValue.get( "type", "" ).asString() );
		secondary.button			= secValue.get( "button", -1 ).asInt(); 
		secondary.negativeButton	= secValue.get( "negative_button", -1 ).asInt();
		secondary.mode				= _GetModeFromString( secValue.get( "mode", "down" ).asString() );
		secondary.axisType			= _GetAxisTypeFromString( secValue.get( "axis_type", "" ).asString() );
		secondary.min				= secValue.get( "min", 0 ).asFloat();
		secondary.max				= secValue.get( "max", 0 ).asFloat();

	}

	EventAction EventAction::FromJson( const json & j )
	{
		return EventAction( j );
	}



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
		eventAction.primary.print();
		eventAction.secondary.print();

		std::string eventName	= eventAction.name;
		_eventActionMap[ eventName ] = eventAction;
	}

	bool InputContext::mapEvent( const std::string & gameEvent, const EventAction ** eventAction ) const
	{
		auto it = _eventActionMap.find( gameEvent );
		if( it == _eventActionMap.end() )
		{
			eventAction = nullptr;
			return false;
		}
		*eventAction = &it->second;
		return true;
	}
}