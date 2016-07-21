// Ghostware Games inc. 2016  -Julian Williams
#pragma once

#include <map>
#include <string>

namespace GG
{
	class GameState;

	class GameStateManager
	{
	public:
		GameStateManager();
		~GameStateManager();

		bool		addGameState( const std::string & stateName, GameState * newState );
		void		showGameState(const std::string & stateName, bool immediately = false );

		void		update( float deltaTime );

	private:
		GameState *	_getCurrentView();

	private:
		std::map<std::string, GameState *>	_stateMap;
		std::string		_currentStateName;
		std::string		_nextStateName;



	};
}