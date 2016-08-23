#pragma once

#include <string>
#include <vector>
#include <array>
#include <unordered_map>

#include <GG/Core/Json.h>

#include "InputContext.h"

namespace GG
{
	struct PersistentState
	{
		PersistentState()
		{
			mouseX = 0;
			mouseY = 0;
			touchX.fill( 0 );
			touchY.fill( 0 );
			touchMode.fill( 0 );
		}

		int32	mouseX;
		int32	mouseY;
		std::array< int32, EventAction::kMaxTouchId > touchX;
		std::array< int32, EventAction::kMaxTouchId > touchY;
		std::array< int32, EventAction::kMaxTouchId > touchMode;
	};


	class InputSystem
	{
	public:
		InputSystem( uint width = 1, uint height = 1);
		~InputSystem();

		void	init( const json & j );
		void	shutdown();

		void	setWindowBounds( uint width, uint height );

		void	update();

		bool	checkAction( const std::string & eventName )	const;
		float	getAxis( const std::string & axisName )			const;

		bool	getTouchPosition( uint touchId, uint & x, uint & y ) const;
		int32	getTouchMode( uint touchId ) const;

	private:
		bool	_checkKeyButton( const EventAction & a )		const;
		bool	_checkPointerButton( const EventAction &  a )	const;
		bool	_checkTouch( const EventAction &  a )			const;
		bool	_getAxis( const EventAction & a, float & axis ) const;

		bool	_scaleMouseAxis( 
			const InputEvent & e, 
			float & outputRange,
			uint axis, 
			uint oldAxis, 
			uint scale ) const;

		bool	_scaleTouchAxis( 
			const InputEvent & e,
			float & outputRange,
			uint axis,
			uint oldAxis,
			uint scale ) const;

		bool	_scaleKeyAxis( const InputEvent & e, float & outputRange ) const;

	private:
		static int32	_GetPointerMotion( void * systemData, void * userData );

	private:

		uint	_width;
		uint	_height;

		PersistentState	_previousState;
		PersistentState	_currentState;

		std::vector< InputContext >	_contextList;

	};
}