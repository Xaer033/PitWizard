
#include "InputSystem.h"

#include <string>
#include <vector>

#include <GG/Core/Json.h>
#include <GG/Core/Log.h>

#include "InputDefines.h"
#include "EventAction.h"
#include "InputContext.h"

namespace GG
{
	InputSystem::InputSystem( uint width, uint height ) : 
		_width(width), _height(height)
	{
		//s3ePointerRegister( s3ePointerCallback::S3E_POINTER_MOTION_EVENT, InputSystem::_GetPointerMotion, ( void* )this );
	}

	InputSystem::~InputSystem()
	{
		//s3ePointerUnRegister( s3ePointerCallback::S3E_POINTER_MOTION_EVENT, InputSystem::_GetPointerMotion );

	}

	void InputSystem::init( const json & j )
	{
		for( uint i = 0; i < j.size(); ++i )
		{
			_contextList.push_back( InputContext( j[ i ] ) );
		}
	}


	void InputSystem::shutdown()
	{
		_contextList.clear();
	}

	void InputSystem::setWindowBounds( uint width, uint height )
	{
		_width = width;
		_height = height;
	}

	void InputSystem::update()
	{
		// Promote current input to previous 
		_previousState = _currentState;

		//Update the input systems
		s3eKeyboardUpdate();
		s3ePointerUpdate();
		

		_currentState.mouseX		= s3ePointerGetX();
		_currentState.mouseY		= s3ePointerGetY();

		if( s3ePointerGetState( S3E_POINTER_BUTTON_SELECT ) & ( int32 )InputMode::PRESSED )
		{
			_previousState.mouseX = _currentState.mouseX;
			_previousState.mouseY = _currentState.mouseY;
		}

		for( uint i = 0; i < EventAction::kMaxTouchId; ++i )
		{
			_currentState.touchX[ i ]		= s3ePointerGetTouchX( i );
			_currentState.touchY[ i ]		= s3ePointerGetTouchY( i );
			_currentState.touchMode[ i ]	= s3ePointerGetTouchState( i );

			if( _currentState.touchMode[ i ] & ( int32 )InputMode::PRESSED )
			{
				_previousState.touchX[ i ] = _currentState.touchX[ i ];
				_previousState.touchY[ i ] = _currentState.touchY[ i ];
			}
		}
	}

	bool InputSystem::checkAction( const std::string & eventName ) const
	{
		for( uint i = 0; i < _contextList.size(); ++i )
		{
			EventAction a;
			if( !_contextList[ i ].mapEvent( eventName, a ) )
			{
				continue;
			}

			if( _checkKeyButton( a )  || _checkPointerButton( a ) || _checkTouch( a ) )
			{
				return true;
			}
		}

		return false;
	}


	float InputSystem::getAxis( const std::string & axisName ) const
	{
		float range = 0;
		for( uint i = 0; i < _contextList.size(); ++i )
		{
			EventAction a;
			if( !_contextList[ i ].mapEvent( axisName, a ) )
				continue;

			if( _getAxis( a, range ) )
				return range;
		}

		return 0.0f;
	}


	bool InputSystem::getTouchPosition( uint touchId, uint & x, uint & y ) const
	{
		x = _currentState.touchX[ touchId ];
		y = _currentState.touchY[ touchId ];

		return !( s3ePointerGetTouchState( touchId ) & s3ePointerState::S3E_POINTER_STATE_UP );
	}

	int32 InputSystem::getTouchMode( uint touchId ) const
	{
		return _currentState.touchMode[touchId];
	}

// -------------------------------------------------------------------------

	int32 InputSystem::_GetPointerMotion( void * systemData, void * userData )
	{
		InputSystem * inputSystem = ( InputSystem* )userData;
		s3ePointerMotionEvent * eventData = ( s3ePointerMotionEvent* )systemData;

		if( inputSystem == nullptr || eventData == nullptr )
			return -1;

		inputSystem->_currentState.mouseX = eventData->m_x;
		inputSystem->_currentState.mouseY = eventData->m_y;

		return 0;
	}

	bool InputSystem::_checkKeyButton( const EventAction & a ) const
	{
		bool primary = a.primary.type == InputType::KEY_BUTTON &&
			s3eKeyboardGetState( ( s3eKey )a.primary.button ) & ( int32 )a.primary.mode;
		
		bool secondary =  a.secondary.type == InputType::KEY_BUTTON &&
			s3eKeyboardGetState( ( s3eKey )a.secondary.button ) & ( int32 )a.secondary.mode;
		
		return primary || secondary;
	}

	bool InputSystem::_checkPointerButton( const EventAction & a ) const
	{
		bool primary = a.primary.type == InputType::POINTER_BUTTON &&
			s3ePointerGetState( ( s3ePointerButton )a.primary.button ) & ( int32 )a.primary.mode;
		
		bool secondary = a.secondary.type == InputType::POINTER_BUTTON &&
			s3ePointerGetState( ( s3ePointerButton )a.secondary.button ) & ( int32 )a.secondary.mode;
		
		return primary || secondary;
	}

	bool InputSystem::_checkTouch( const EventAction & a ) const
	{
		bool primary = a.primary.type == InputType::TOUCH  &&
			s3ePointerGetTouchState( a.primary.touchId ) & ( int32 )a.primary.mode;

		bool secondary = a.secondary.type == InputType::TOUCH  &&
			s3ePointerGetTouchState( a.secondary.touchId ) & ( int32 )a.secondary.mode;

		return primary || secondary;
	}

	bool InputSystem::_scaleMouseAxis( 
		const InputEvent & e, 
		float & outputRange, 
		uint axis, 
		uint oldAxis, 
		uint scale ) const
	{
		float safeScale = (float)scale < 1.0f ? 1.0f : (float)scale;

		float a		= (float)axis / (float)safeScale;
		float o		= (float)oldAxis / (float)safeScale;

		float delta	= a - o;

		outputRange = ( ( fabsf( delta ) < e.dead ) ? 0.0f : delta ) * e.sensitivity;
		TRACE_DEBUG( "Axis: %f, Prev: %f, Delta: %f, Range: %f", a, o, delta, outputRange );
		return true;
	}

	bool InputSystem::_scaleTouchAxis(
		const InputEvent & e,
		float & outputRange,
		uint axis,
		uint oldAxis,
		uint scale ) const
	{
		if( _currentState.touchMode[ e.touchId ] & S3E_POINTER_STATE_DOWN )
		{
			return _scaleMouseAxis( e, outputRange, axis, oldAxis, scale );
		}

		outputRange = 0;
		return false;
	}

	bool InputSystem::_scaleKeyAxis( const InputEvent & e, float & outputRange ) const
	{
		bool posButton = s3eKeyboardGetState( ( s3eKey )e.button ) & S3E_KEY_STATE_DOWN;
		bool negButton = s3eKeyboardGetState( (s3eKey)e.negativeButton ) & S3E_KEY_STATE_DOWN;
		
		if( !posButton && !negButton )
			return false;

		if( posButton && negButton )
			outputRange = 0;
		else if( posButton )
			outputRange = e.max;
		else if( negButton )
			outputRange = e.min;
		
		outputRange *= e.sensitivity;
		return true;
	}

	bool InputSystem::_getAxis( const EventAction & a, float & axis ) const
	{
		switch( a.primary.axisType )
		{
		case AxisType::MOUSE_X: return _scaleMouseAxis( a.primary, axis, _currentState.mouseX, _previousState.mouseX, _width );
		case AxisType::MOUSE_Y:	return _scaleMouseAxis( a.primary, axis, _currentState.mouseY, _previousState.mouseY, _height );
		case AxisType::TOUCH_DRAG_X: return _scaleTouchAxis( a.primary, axis, _currentState.touchX[ a.primary.touchId ], _previousState.touchX[ a.primary.touchId ], _width );
		case AxisType::TOUCH_DRAG_Y: return _scaleTouchAxis( a.primary, axis, _currentState.touchY[ a.primary.touchId ], _previousState.touchY[ a.primary.touchId ], _height );
		case AxisType::KEY_BUTTON: return _scaleKeyAxis( a.primary, axis );
		}

		switch( a.secondary.axisType )
		{
		case AxisType::MOUSE_X: return _scaleMouseAxis( a.secondary, axis, _currentState.mouseX, _previousState.mouseX, _width );
		case AxisType::MOUSE_Y:	return _scaleMouseAxis( a.secondary, axis, _currentState.mouseY, _previousState.mouseY, _height );
		case AxisType::TOUCH_DRAG_X: return _scaleTouchAxis( a.secondary, axis, _currentState.touchX[ a.secondary.touchId ], _previousState.touchX[ a.secondary.touchId ], _width );
		case AxisType::TOUCH_DRAG_Y: return _scaleTouchAxis( a.secondary, axis, _currentState.touchY[ a.secondary.touchId ], _previousState.touchY[ a.secondary.touchId ], _height );
		case AxisType::KEY_BUTTON: return _scaleKeyAxis( a.secondary, axis );
		}

		return false;
	}
}