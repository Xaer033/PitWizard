#pragma once

namespace GG
{
	class GameState
	{
	public:
		GameState();
		virtual ~GameState();

		virtual void		init()						= 0;

		virtual void		update( float deltaTime )	= 0;
		virtual bool		transitionIn()				= 0;
		virtual bool		transitionOut()				= 0;

		virtual void		shutdown()					= 0;

	protected: 
		bool				_isTransitioningIn;
		bool				_isTransitioningOut;
	};
}