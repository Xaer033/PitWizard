// Ghostware Games inc. 2016  -Julian Williams
#pragma once

#include <string>
#include <GG/Core/Json.h>
#include "InputDefines.h"

namespace GG
{
	struct InputEvent
	{
		InputType		type;
		InputMode		mode;

		int				button;
		int				negativeButton;

		int				touchId;

		AxisType		axisType;
		float			min;
		float			max;
		float			dead;
		float			sensitivity;
	};


	class EventAction
	{
	public:
		static const uint	kMaxTouchId = S3E_POINTER_TOUCH_MAX;

		static EventAction	FromJson( const json & j );

	public:
		EventAction();
		EventAction( const json & j );

		std::string		name;

		InputEvent		primary;
		InputEvent		secondary;

	private:

		static InputType	_GetTypeFromString( const std::string & typeStr );
		static InputMode	_GetModeFromString( const std::string & modeStr );
		static AxisType		_GetAxisTypeFromString( const std::string & axisType );
		static uint			_GetKeyFromValue( const json & value );
	};
}
