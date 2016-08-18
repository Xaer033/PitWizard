
#include "InputSystem.h"

#include <functional>
#include <unordered_map>
#include <set>
#include <string>
#include <vector>

#include <GG/Core/Json.h>

#include "RawInput.h"
#include "InputContext.h"

namespace GG
{
	InputSystem::InputSystem()
	{
	}

	InputSystem::~InputSystem()
	{
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

	void InputSystem::update()
	{
		//Update the input systems
		s3eKeyboardUpdate();
		s3ePointerUpdate();
	}

	bool InputSystem::checkAction( const std::string & eventName ) const
	{
		for( uint i = 0; i < _contextList.size(); ++i )
		{
			EventAction * a = nullptr;
			if( !_contextList[ i ].mapEvent( eventName, &a ) )
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
		for( uint i = 0; i < _contextList.size(); ++i )
		{
			EventAction * a = nullptr;
			if( !_contextList[ i ].mapEvent( axisName, &a ) )
			{
				continue;
			}

			switch( a->primary.axisType )
			{
			case AxisType::MOUSE_X:

			
			}
			
		}

		return 0.0f;
	}

// -------------------------------------------------------------------------

	bool InputSystem::_checkKeyButton( const EventAction * a ) const
	{
		bool primary = a->primary.type == InputType::KEY_BUTTON &&
			s3eKeyboardGetState( ( s3eKey )a->primary.button ) & ( int32 )a->primary.mode;
		
		bool secondary =  a->secondary.type == InputType::KEY_BUTTON &&
			s3eKeyboardGetState( ( s3eKey )a->secondary.button ) & ( int32 )a->secondary.mode;
		
		return primary || secondary;
	}

	bool InputSystem::_checkPointerButton( const EventAction * a ) const
	{

		bool primary = a->primary.type == InputType::POINTER_BUTTON &&
			s3ePointerGetState( ( s3ePointerButton )a->primary.button ) & ( int32 )a->primary.mode;
		
		bool secondary = a->secondary.type == InputType::POINTER_BUTTON &&
			s3ePointerGetState( ( s3ePointerButton )a->secondary.button ) & ( int32 )a->secondary.mode;
		
		return primary || secondary;
	}

	bool InputSystem::_checkTouch( const EventAction * a ) const
	{
		bool primary = a->primary.type == InputType::TOUCH  &&
			s3ePointerGetTouchState( a->primary.touchId ) & ( int32 )a->primary.mode;

		bool secondary = a->secondary.type == InputType::TOUCH  &&
			s3ePointerGetTouchState( a->secondary.touchId ) & ( int32 )a->secondary.mode;

		return primary || secondary;
	}
}