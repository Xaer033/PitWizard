
#include "EventAction.h"

#include <string>
#include <GG/Core/Json.h>
#include <GG/Core/Log.h>

#include "InputDefines.h"

namespace GG
{
	InputType EventAction::_GetTypeFromString( const std::string & typeStr )
	{
		if( typeStr == "key_button" ) { return InputType::KEY_BUTTON; }
		else if( typeStr == "pointer_button" ) { return InputType::POINTER_BUTTON; }
		else if( typeStr == "touch" ) { return InputType::TOUCH; }
		else if( typeStr == "axis" ) { return InputType::AXIS; }

		if( !typeStr.empty() )
		{
			TRACE_WARNING( "Input Type: %s not supported!", typeStr );
		}

		return InputType::NONE;
	}

	AxisType EventAction::_GetAxisTypeFromString( const std::string & axisType )
	{
		if( axisType == "mouse_x" ) { return AxisType::MOUSE_X; }
		else if( axisType == "mouse_y" ) { return AxisType::MOUSE_Y; }
		else if( axisType == "key_button" ) { return AxisType::KEY_BUTTON; }
		else if( axisType == "touch_drag_x" ) { return AxisType::TOUCH_DRAG_X; }
		else if( axisType == "touch_drag_y" ) { return AxisType::TOUCH_DRAG_Y; }

		if( !axisType.empty() )
		{
			TRACE_WARNING( "Axis Type: %s not supported!", axisType );
		}

		return AxisType::NONE;
	}

	InputMode EventAction::_GetModeFromString( const std::string & modeStr )
	{
		if( modeStr == "up" ) { return InputMode::UP; }
		else if( modeStr == "down" ) { return InputMode::DOWN; }
		else if( modeStr == "pressed" ) { return InputMode::PRESSED; }
		else if( modeStr == "released" ) { return InputMode::RELEASED; }

		return InputMode::UP;
	}

	uint EventAction::_GetKeyFromValue( const json & value )
	{

		if( value.type() == Json::ValueType::uintValue ||
			value.type() == Json::ValueType::intValue )
		{
			return value.asUInt();
		}

		std::string key = value.asString();
		std::transform( key.begin(), key.end(), key.begin(), ::tolower );


		if( key == "esc" ) { return s3eKeyEsc; }
		if( key == "tab" ) { return s3eKeyTab; }
		if( key == "enter" ) { return s3eKeyEnter; }
		if( key == "space" ) { return s3eKeySpace; }
		if( key == "backspace" ) { return s3eKeyBackspace; }
		if( key == "left_shift" ) { return s3eKeyLeftShift; }
		if( key == "left_control" ) { return s3eKeyLeftControl; }
		if( key == "left_alt" ) { return s3eKeyLeftAlt; }
		if( key == "right_shift" ) { return s3eKeyRightShift; }
		if( key == "right_control" ) { return s3eKeyRightControl; }
		if( key == "right_alt" ) { return s3eKeyRightAlt; }

		if( key == "up" ) { return s3eKeyUp; }
		if( key == "down" ) { return s3eKeyDown; }
		if( key == "left" ) { return s3eKeyLeft; }
		if( key == "right" ) { return s3eKeyRight; }

		if( key == "a" ) { return s3eKeyA; }
		if( key == "b" ) { return s3eKeyB; }
		if( key == "c" ) { return s3eKeyC; }
		if( key == "d" ) { return s3eKeyD; }
		if( key == "e" ) { return s3eKeyE; }
		if( key == "f" ) { return s3eKeyF; }
		if( key == "g" ) { return s3eKeyG; }
		if( key == "h" ) { return s3eKeyH; }
		if( key == "i" ) { return s3eKeyI; }
		if( key == "j" ) { return s3eKeyJ; }
		if( key == "k" ) { return s3eKeyK; }
		if( key == "l" ) { return s3eKeyL; }
		if( key == "m" ) { return s3eKeyM; }
		if( key == "n" ) { return s3eKeyN; }
		if( key == "o" ) { return s3eKeyO; }
		if( key == "p" ) { return s3eKeyP; }
		if( key == "q" ) { return s3eKeyQ; }
		if( key == "r" ) { return s3eKeyR; }
		if( key == "s" ) { return s3eKeyS; }
		if( key == "t" ) { return s3eKeyT; }
		if( key == "u" ) { return s3eKeyU; }
		if( key == "v" ) { return s3eKeyV; }
		if( key == "w" ) { return s3eKeyW; }
		if( key == "x" ) { return s3eKeyX; }
		if( key == "y" ) { return s3eKeyY; }
		if( key == "z" ) { return s3eKeyZ; }

		if( key == "0" ) { return s3eKey0; }
		if( key == "1" ) { return s3eKey1; }
		if( key == "2" ) { return s3eKey2; }
		if( key == "3" ) { return s3eKey3; }
		if( key == "4" ) { return s3eKey4; }
		if( key == "5" ) { return s3eKey5; }
		if( key == "6" ) { return s3eKey6; }
		if( key == "7" ) { return s3eKey7; }
		if( key == "8" ) { return s3eKey8; }
		if( key == "9" ) { return s3eKey9; }

		if( key == "f1" ) { return s3eKeyF1; }
		if( key == "f2" ) { return s3eKeyF2; }
		if( key == "f3" ) { return s3eKeyF3; }
		if( key == "f4" ) { return s3eKeyF4; }
		if( key == "f5" ) { return s3eKeyF5; }
		if( key == "f6" ) { return s3eKeyF6; }
		if( key == "f7" ) { return s3eKeyF7; }
		if( key == "f8" ) { return s3eKeyF8; }
		if( key == "f9" ) { return s3eKeyF9; }
		if( key == "f10" ) { return s3eKeyF10; }
		if( key == "f11" ) { return s3eKeyF11; }
		if( key == "f12" ) { return s3eKeyF12; }

		if( key == "numpad_0" ) { return s3eKeyNumPad0; }
		if( key == "numpad_1" ) { return s3eKeyNumPad1; }
		if( key == "numpad_2" ) { return s3eKeyNumPad2; }
		if( key == "numpad_3" ) { return s3eKeyNumPad3; }
		if( key == "numpad_4" ) { return s3eKeyNumPad4; }
		if( key == "numpad_5" ) { return s3eKeyNumPad5; }
		if( key == "numpad_6" ) { return s3eKeyNumPad6; }
		if( key == "numpad_7" ) { return s3eKeyNumPad7; }
		if( key == "numpad_8" ) { return s3eKeyNumPad8; }
		if( key == "numpad_9" ) { return s3eKeyNumPad9; }

		return 0;
	}

	EventAction::EventAction()
	{
	}

	EventAction::EventAction( const json & j )
	{
		name	= j.get( "name", Json::nullValue ).asString();

		auto primValue				= j.get( "primary", json::nullRef );
		primary.type				= _GetTypeFromString( primValue.get( "type", "" ).asString() );
		primary.button				= _GetKeyFromValue( primValue.get( "button", json::nullRef ) );
		primary.negativeButton		= _GetKeyFromValue( primValue.get( "negative_button", json::nullRef ) );
		primary.mode				= _GetModeFromString( primValue.get( "mode", "down" ).asString() );
		primary.axisType			= _GetAxisTypeFromString( primValue.get( "axis_type", "" ).asString() );
		primary.touchId				= primValue.get( "touchId", 0 ).asUInt();
		primary.min					= primValue.get( "min", -1.0f ).asFloat();
		primary.max					= primValue.get( "max", 1.0f ).asFloat();
		primary.dead				= primValue.get( "dead", 0.001f ).asFloat();
		primary.sensitivity			= primValue.get( "sensitivity", 1.0f ).asFloat();

		auto secValue				= j.get( "secondary", json::nullRef );
		secondary.type				= _GetTypeFromString( secValue.get( "type", "" ).asString() );
		secondary.button			= _GetKeyFromValue( secValue.get( "button", json::nullRef ) );
		secondary.negativeButton	= _GetKeyFromValue( secValue.get( "negative_button", json::nullRef ) );
		secondary.mode				= _GetModeFromString( secValue.get( "mode", "down" ).asString() );
		secondary.axisType			= _GetAxisTypeFromString( secValue.get( "axis_type", "" ).asString() );
		secondary.touchId			= secValue.get( "touchId", 0 ).asUInt();
		secondary.min				= secValue.get( "min", 0 ).asFloat();
		secondary.max				= secValue.get( "max", 0 ).asFloat();
		secondary.dead				= secValue.get( "dead", 0.001f ).asFloat();
		secondary.sensitivity		= secValue.get( "sensitivity", 1.0f ).asFloat();
	}

	EventAction EventAction::FromJson( const json & j )
	{
		return EventAction( j );
	}
}
