
#include "GameStateManager.h"

#include <map>
#include <string>
#include <s3e.h>
#include <GG/Core/Log.h>

#include "GameState.h"


namespace GG
{
	GameStateManager::GameStateManager()
	{

	}

	GameStateManager::~GameStateManager()
	{

	}

	bool GameStateManager::addGameState(	const std::string & name, 
											GameState * newView )
	{
		if( name.size() == 0 || newView == nullptr )
		{
			TRACE_WARNING( "Could not add game view, input invalid!" );
			return false;
		}
		_stateMap[ name ] = newView;
		return true;
	}

	void GameStateManager::update( float deltaTime )
	{
		GameState * currentState = _getCurrentView();
		if( currentState != nullptr )
		{
			currentState->update( deltaTime );
		}

		//if( currentView->_is )
	}

	void GameStateManager::showGameState( const std::string & viewName, bool immediately )
	{
		if( immediately )
		{
			GameState * currentState = _getCurrentView();
			if( currentState != nullptr )
			{
				currentState->shutdown();
			}
			_currentStateName = viewName;
			_stateMap[ viewName ]->init();
		}
		else
		{
			_nextStateName = viewName;
			_stateMap[ _currentStateName ]->transitionOut();
		}
	}

	GameState *	GameStateManager::_getCurrentView()
	{
		auto it = _stateMap.find( _currentStateName );
		if( it != _stateMap.end() )
		{
			return it->second;
		}
		return nullptr;
	}

}