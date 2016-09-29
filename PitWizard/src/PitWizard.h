#pragma once

#include <GG/GameLayer.h>
#include <GG/Input.h>

class CIwModel;

class PitWizard : public GG::Application
{
public:
	PitWizard();
	~PitWizard();

	virtual void		init()				final;
	virtual void		doGameLoop()		final;
	virtual void		shutdown()			final;

	GG::InputSystem	*	inputSystem;

private:
	void				_setupLoggers();
	const std::string	_getFileLogName() const;

	uint				_deviceWidth;
	uint				_deviceHeight;
	
	GG::GameStateManager	_gameStateManager;
	CIwModel * _test;
};
