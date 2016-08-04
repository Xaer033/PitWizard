
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
	GG::SceneNode boxTransform;

	float aspect = ( float )IwGxGetScreenWidth() / ( float )IwGxGetScreenHeight();
	
	GG::Camera cam;
	cam.sceneNode.setPosition( GG::Vector3( 2, 5, 10 ) );
	cam.sceneNode.lookAt( boxTransform.getWorldPosition(), -GG::Vector3::g_AxisY );
	cam.setPerspective( 60.0f, aspect, 0.1f, 300.0f );
	cam.setViewport( 0, 0, 1, 1 );
	cam.setClearMode( GG::ClearMode::Depth | GG::ClearMode::Color );
	cam.setClearColor( GG::Vector4( 0.1f, 0.03f, 0.14f, 1 ) );


	GG::Camera cam2;
	cam2.sceneNode.setPosition( boxTransform.getWorldPosition() + boxTransform.left() * 4.0f );
	cam2.sceneNode.lookAt( &boxTransform, -GG::Vector3::g_AxisY );
	cam2.setPerspective( 60.0f, aspect, 0.1f, 300.0f );
	cam2.setViewport( 0, 0, 0.2f, 0.2f );
	cam2.setClearMode( GG::ClearMode::Depth  );
	cam2.setClearColor( GG::Vector4( 0.25f, 0.3f, 0.2f, 1 ) );


	GG::RenderFactory renderFactory;
	bool follow = false;

	float _angle = 0;
	GG::SceneNode parent;
	parent.setPosition( GG::Vector3( -4.0f, 0, 0 ) );
	boxTransform.setParent( &parent );
	CIwGxSurface fbo;
	fbo.CreateSurface( nullptr, IwGxGetScreenWidth(), IwGxGetScreenHeight(), CIwGxSurface::CREATE_8888_SURFACE_F );
	
	
	GG::SceneNode cool;
	cool.setPosition( GG::Vector3( 1.0f, 2.0f, -3.0f ) );
	cool.setParent( &boxTransform );

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
		else if( s3eKeyboardGetState( s3eKeyS ) & S3E_KEY_STATE_DOWN )
		{
			boxTransform.rotate( -0.01f, GG::Vector3( 0, 1, 0 ) );
		}
		
		if( s3eKeyboardGetState( s3eKeyDown ) & S3E_KEY_STATE_DOWN )
		{
			boxTransform.translate( boxTransform.back() * 0.1f );
		}
		if( s3eKeyboardGetState( s3eKeyUp ) & S3E_KEY_STATE_DOWN )
		{
			boxTransform.translate( boxTransform.forward() * 0.1f );
		}

		if( s3eKeyboardGetState( s3eKeyLeft ) & S3E_KEY_STATE_DOWN )
		{
			parent.rotate( 0.01f, GG::Vector3( 0, 1, 0 ) );
		}
		else if( s3eKeyboardGetState( s3eKeyRight ) & S3E_KEY_STATE_DOWN )
		{
			parent.rotate( -0.01f, GG::Vector3( 0, 1, 0 ) );
		}

		if( s3eKeyboardGetState( s3eKeyF ) & S3E_KEY_STATE_PRESSED )
		{ 
			follow = !follow;
		}

		if( s3eKeyboardGetState( s3eKeyB ) & S3E_KEY_STATE_PRESSED )
		{
			boxTransform.setParent( nullptr );
		}

		if( s3eKeyboardGetState( s3eKeyP ) & S3E_KEY_STATE_PRESSED )
		{
			boxTransform.setParent( &parent );
		}

		if( follow )
		{
			cam.sceneNode.lookAt( &boxTransform );
			/*GG::LOG_DEBUG(
				"Box Position: %s\n", 
				GG::ToString( boxTransform.getWorldPosition() )
			);*/
		}
		
		GG::LOG_DEBUG( "Cool Matrix:\n%s\n", GG::ToString( boxTransform.modelToWorldMatrix() ) );
		

		renderFactory.addCommand( creepMat, _test, cool.modelToWorldMatrix() );
		renderFactory.addCommand( creepMat, _test, boxTransform.modelToWorldMatrix() );
		renderFactory.addCommand( creepMat, _test, parent.modelToWorldMatrix() );

		//fbo.MakeCurrent();
		renderFactory.renderAll( &cam );
		renderFactory.renderAll( &cam2 );

		renderFactory.clearAllCommands();
		
		// Standard EGL-style flush of drawing to the surface
		IwGxFlush();
		//fbo.MakeDisplayCurrent();

		// Standard EGL-style flipping of double-buffers
		IwGxSwapBuffers();

		// Sleep for 0ms to allow the OS to process events etc.
		s3eDeviceYield( 0 );

	}
}
