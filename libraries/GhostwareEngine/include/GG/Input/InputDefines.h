#pragma once
#include <GG/Core/Types.h>
#include <s3eKeyboard.h>
#include <s3ePointer.h>

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
		TOUCH_DRAG_X,
		TOUCH_DRAG_Y,
	};

}