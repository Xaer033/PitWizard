#pragma once

#include <GG/GameLayer.h>
#include <GG/Input.h>

class CIwModel;

class PitWizard : public GG::IApplication
{
public:
	PitWizard();
	~PitWizard();

	virtual void		init(int argc, char** argv)	final;
	virtual void		doGameLoop()				final;
	virtual void		shutdown()					final;

	std::unique_ptr<GG::InputSystem>	inputSystem;

private:
	void				_setupLoggers();
	const std::string	_getFileLogName() const;

	uint				_deviceWidth;
	uint				_deviceHeight;
	
	GG::GameStateManager	_gameStateManager;
	CIwModel * _test;
};
