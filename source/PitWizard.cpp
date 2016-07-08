
#include "PitWizard.h"

#include "s3e.h"
#include "IwDebug.h"
#include "IwGx.h"
#include <Photon-cpp/inc/PhotonPeer.h>

#include <GG/Application.h>
#include <GG/GameStateManager.h>


void PitWizard::init()
{
	Application::init();
	_gameStateManager.addGameState( "Intro",					nullptr );
	_gameStateManager.addGameState( "MainMenu",				nullptr );
	_gameStateManager.addGameState( "SinglePlayer",			nullptr );
	_gameStateManager.addGameState( "SinglePlayerEndScreen",	nullptr );
	_gameStateManager.addGameState( "MultiPlayer",			nullptr );
	_gameStateManager.addGameState( "MultiPlayerEndScreen",	nullptr );

	IwGxSetColClear( 125, 126, 128, 0 );
}

void PitWizard::shutdown()
{
	Application::shutdown();
}

void PitWizard::doGameLoop()
{
	// Loop forever, until the user or the OS performs some action to quit the app
	while( !s3eDeviceCheckQuitRequest() )
	{
		//Update the input systems
		s3eKeyboardUpdate();
		s3ePointerUpdate();


		// Clear the surface
		IwGxClear( IW_GX_COLOUR_BUFFER_F | IW_GX_DEPTH_BUFFER_F );

		// Your rendering/app code goes here.

		// Standard EGL-style flush of drawing to the surface
		IwGxFlush();
		// Standard EGL-style flipping of double-buffers
		IwGxSwapBuffers();

		// Sleep for 0ms to allow the OS to process events etc.
		s3eDeviceYield( 0 );
	}
}
