#pragma once

#include <string>
#include <vector>
#include <unordered_map>

#include <GG/Core/Json.h>

#include "RawInput.h"
#include "InputContext.h"

namespace GG
{
	class InputSystem
	{
	public:
		InputSystem();
		~InputSystem();

		void	init( const json & j );
		void	shutdown();

		void	update();

		bool	checkAction( const std::string & eventName ) const;
		float	getAxis( const std::string & axisName )		const;

	private:
		bool	_testKeyButton( const EventAction * a )		const;
		bool	_testPointerButton( const EventAction * a ) const;
		bool	_testTouch( const EventAction * a )			const;


	private:
		std::vector< InputContext >				_contextList;
		std::unordered_map< std::string, float> _oldAxisValues;

	};
}