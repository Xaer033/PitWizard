#pragma once

#include <GG/Application.h>
#include <GG/GameStateManager.h>

class PitWizard : public GG::Application
{
public:
	virtual void		init()				final;
	virtual void		doGameLoop()		final;
	virtual void		shutdown()			final;

private:
	GG::GameStateManager		_gameStateManager;
};