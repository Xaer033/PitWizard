#pragma once

#include <unordered_map>

#include <GG/Core/Json.h>
#include <GG/Core/Log.h>

#include "RawInput.h"

namespace GG
{
	enum class InputType
	{
		NONE = 0,
		KEY_BUTTON,
		POINTER_BUTTON,
		TOUCH,
		AXIS,
	};

	enum class InputMode
	{
		UP = S3E_KEY_STATE_UP,
		DOWN = S3E_KEY_STATE_DOWN,
		PRESSED = S3E_KEY_STATE_PRESSED,
		RELEASED = S3E_KEY_STATE_RELEASED,
	};
	
	enum class AxisType
	{
		NONE = 0,
		MOUSE_X,
		MOUSE_Y,
		KEY_BUTTON,
		POINTER_BUTTON,
		TOUCH_DRAG_X,
		TOUCH_DRAG_Y,
	};

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

		void print() const
		{
			TRACE_DEBUG(
				"InputEvent: %d, %d, %d, %d, %f, %f",
				type,
				button,
				mode,
				min,
				max
			);
		}
	};


	class EventAction
	{
	public:
		static EventAction FromJson( const json & j );

	public:
		EventAction(){}
		EventAction( const json & j );

		std::string		name;
		InputEvent		primary;
		InputEvent		secondary;

	private:

		static InputType _GetTypeFromString( const std::string & typeStr );
		static InputMode _GetModeFromString( const std::string & modeStr );
		static AxisType  _GetAxisTypeFromString( const std::string & axisType );

	};


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

		bool mapEvent( const std::string & gameEvent, const EventAction ** eventAction ) const;

	private:

		std::string		_name;
		int				_priority;


		std::unordered_map< std::string, EventAction > _eventActionMap;
	};
	
	
}