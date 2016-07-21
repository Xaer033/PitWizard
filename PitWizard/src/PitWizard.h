#pragma once

#include "GG/GameLayer/Application.h"
#include "GG/GameLayer/GameStateManager.h"


class CIwModel;

class PitWizard : public GG::Application
{
public:
	virtual void		init()				final;
	virtual void		doGameLoop()		final;
	virtual void		shutdown()			final;

private:
	GG::GameStateManager		_gameStateManager;
	CIwModel * _test;
};
