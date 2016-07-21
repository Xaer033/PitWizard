
#include "PitWizard.h"

#include <iostream>

#include "s3e.h"
#include "IwDebug.h"
#include "IwResManager.h"

#include <Photon-cpp/inc/PhotonPeer.h>


#include <GG/GameLayer.h>
#include <GG/Graphics.h>
#include <GG/Core.h>


CIwMaterial * creepMat;

void PitWizard::init()
{
	Application::init();
	_gameStateManager.addGameState( "Intro",					nullptr );
	_gameStateManager.addGameState( "MainMenu",					nullptr );
	_gameStateManager.addGameState( "SinglePlayer",				nullptr );
	_gameStateManager.addGameState( "SinglePlayerEndScreen",	nullptr );
	_gameStateManager.addGameState( "MultiPlayer",				nullptr );
	_gameStateManager.addGameState( "MultiPlayerEndScreen",		nullptr );

	
	
	CIwResGroup * group = IwGetResManager()->LoadGroup( "./creeps/creep_01.group" );
	_test		= ( CIwModel* )group->GetResNamed( "creep_01", IW_GRAPHICS_RESTYPE_MODEL );
}

void PitWizard::shutdown()
{
	Application::shutdown();
}

void PitWizard::doGameLoop()
{
	GG::Transform boxTransform;

	float aspect = ( float )IwGxGetScreenWidth() / ( float )IwGxGetScreenHeight();
	
	GG::Camera cam;
	cam.transform.setPosition( GG::Vector3( 2, 5, 10 ) );
	cam.transform.lookAt( boxTransform.getPosition(), -GG::Vector3::g_AxisY );
	cam.setPerspective( 60.0f, aspect, 0.1f, 300.0f );
	cam.setViewport( 0, 0, 1, 1 );
	cam.setClearMode( GG::ClearMode::Depth | GG::ClearMode::Color );
	cam.setClearColor( GG::Vector4( 0.1, 0.03, 0.14f, 1 ) );


	GG::Camera cam2;
	cam2.transform.setPosition( GG::Vector3( 2, 5, 10 ) );
	cam2.transform.lookAt( boxTransform.getPosition(), -GG::Vector3::g_AxisY );
	cam2.setPerspective( 60.0f, aspect, 0.1f, 300.0f );
	cam2.setViewport( 0, 0, 0.2f, 0.2f );
	cam2.setClearMode( GG::ClearMode::Depth  );
	cam2.setClearColor( GG::Vector4( 0.25, 0.3, 0.2f, 1 ) );


	GG::RenderFactory renderFactory;

	float _angle = 0;

	// Loop forever, until the user or the OS performs some action to quit the app
	while( !s3eDeviceCheckQuitRequest() )
	{
		//Update the input systems
		s3eKeyboardUpdate();
		s3ePointerUpdate();

		if( s3eKeyboardGetState( s3eKeyA ) & S3E_KEY_STATE_DOWN )
		{
			boxTransform.rotate( 0.01f, GG::Vector3( 0, 1, 0 ));
		}

		renderFactory.addCommand( creepMat, _test, &boxTransform.getMatrix() );

		renderFactory.renderAll( &cam );
		renderFactory.renderAll( &cam2 );

		renderFactory.clearAllCommands();


		// Standard EGL-style flush of drawing to the surface
		IwGxFlush();

		// Standard EGL-style flipping of double-buffers
		IwGxSwapBuffers();

		// Sleep for 0ms to allow the OS to process events etc.
		s3eDeviceYield( 0 );

	}
}
