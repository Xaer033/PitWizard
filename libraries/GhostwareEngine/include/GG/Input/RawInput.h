#pragma once
#include <GG/Core/Types.h>
#include <s3eKeyboard.h>
#include <s3ePointer.h>

namespace GG
{
	typedef s3eKey				KeyButton;
	typedef s3ePointerButton	PointerButton;

	typedef uint				RawAxis;

	/*enum RawAxis
	{
		MOUSE_X = 1,
		MOUSE_Y,
	};*/

	class RawInput
	{
	public:
		// Callback function to handle pressing/releasing the screen or a mouse button
		//int32 PointerButtonEventCallback( s3ePointerEvent* pEvent, void* pUserData );

		// Callback function to handle drags on the touchscreen/mouse movements
		//int32 PointerMotionEventCallback( s3ePointerMotionEvent* pEvent, void* pUserData );
	};
}