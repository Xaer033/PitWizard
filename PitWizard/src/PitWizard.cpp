
#include "PitWizard.h"

#include <iostream>
#include <chrono>


#include "s3e.h"
#include "IwDebug.h"

//#include <Photon-cpp/inc/PhotonPeer.h>

#include <GG/GameLayer.h>
#include <GG/Graphics.h>
#include <GG/Core.h>
#include <GG/Input.h>

using namespace GG;



void PitWizard::init()
{
	Application::init();
	_setupLoggers();

	_gameStateManager.addGameState( "Intro",					nullptr );
	_gameStateManager.addGameState( "MainMenu",					nullptr );
	_gameStateManager.addGameState( "SinglePlayer",				nullptr );
	_gameStateManager.addGameState( "SinglePlayerEndScreen",	nullptr );
	_gameStateManager.addGameState( "MultiPlayer",				nullptr );
	_gameStateManager.addGameState( "MultiPlayerEndScreen",		nullptr );

	
	
	CIwResGroup * group = IwGetResManager()->LoadGroup( "./resources/creeps/creep_02.group" );
	_test		= ( CIwModel* )group->GetResNamed( "Cube", IW_GRAPHICS_RESTYPE_MODEL );
	
	json config = JsonFromFile( "configs/input_config.json" );
	inputSystem = new InputSystem( IwGxGetScreenWidth(), IwGxGetScreenHeight() );
	inputSystem->init( config );
}

void PitWizard::shutdown()
{	
	inputSystem->shutdown();
	delete( inputSystem );

	Log::Shutdown();
	Application::shutdown();
}


void PitWizard::doGameLoop()
{
	GG::SceneNode boxTransform;

	float aspect =	( float )IwGxGetScreenWidth() / 
					( float )IwGxGetScreenHeight();
	
	GG::SceneNode camNode_1;
	GG::Camera cam;
	camNode_1.attachObject( &cam );
	camNode_1.setPosition( GG::Vector3( 2, 5, 10 ) );
	camNode_1.lookAt( &boxTransform );
	cam.setPerspective( 60.0f, aspect, 0.1f, 300.0f );
	cam.setViewport( 0, 0, 1, 1 );
	cam.setClearMode( GG::ClearMode::Depth | GG::ClearMode::Color );
	cam.setClearColor( GG::Vector4( 0.1f, 0.03f, 0.14f, 1 ) );


	GG::SceneNode camNode_2;
	GG::Camera cam2;

	camNode_2.attachObject( &cam2 );
	camNode_2.setPosition( boxTransform.getWorldPosition() + boxTransform.left() * 4.0f );
	camNode_2.lookAt( &boxTransform );
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
	
	
	GG::SceneNode cool;
	cool.setPosition( GG::Vector3( 1.0f, 2.0f, -3.0f ) );
	cool.setParent( &boxTransform );

	GG::Mesh mesh( _test, nullptr );

	// Loop forever, until the user or the OS performs some action to quit the app
	while( !s3eDeviceCheckQuitRequest() )
	{
		//Update the input systems
		inputSystem->update();

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
		
		TRACE_DEBUG( "Axis: %f", inputSystem->getAxis( "RotateX" ) );
		camNode_1.rotate( inputSystem->getAxis( "RotateX" ), -Vector3::g_AxisY );

		float walk		= inputSystem->getAxis( "Walk" );
		float strafe	= inputSystem->getAxis( "Strafe" );

		TRACE_DEBUG( "Walk: %f", walk );
		TRACE_DEBUG( "Strafe: %f", strafe );

		Vector3 move = ( camNode_1.forward() * walk ) + ( camNode_1.right() * strafe );
		move = move.GetLengthSquared() > 0.001f ? move.GetNormalised() : move;
		camNode_1.translate( move * 0.4f);

		if( s3eKeyboardGetState( s3eKeyF ) & S3E_KEY_STATE_PRESSED )
		{ 
			follow = !follow;
		}

		if( s3eKeyboardGetState( s3eKeyEsc ) & S3E_KEY_STATE_PRESSED )
		{
			s3eDeviceRequestQuit();
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
			camNode_1.lookAt( &boxTransform );
		}
		
		renderFactory.addCommand( mesh.material, mesh.geometry, cool.modelToWorldMatrix() );
		renderFactory.addCommand( mesh.material, mesh.geometry, boxTransform.modelToWorldMatrix() );
		renderFactory.addCommand( mesh.material, mesh.geometry, parent.modelToWorldMatrix() );

		renderFactory.renderAll( &cam );
		renderFactory.renderAll( &cam2 );

		renderFactory.clearAllCommands();
		

		//// Standard EGL-style flush of drawing to the surface
		IwGxFlush();
	
		//// Standard EGL-style flipping of double-buffers
		IwGxSwapBuffers();

		// Sleep for 0ms to allow the OS to process events etc.
		s3eDeviceYield( 0 );
	}
}

void PitWizard::_setupLoggers()
{
	const std::string filePath = _getFileLogName();

	Log::GetInstance()->registerLogger( new FileLogger( filePath ) );
	Log::GetInstance()->registerLogger( new ConsoleLogger() );
}

const std::string PitWizard::_getFileLogName() const
{
	auto now = std::chrono::system_clock::now();
	const std::time_t now_time = std::chrono::system_clock::to_time_t( now );
	auto t = localtime( &now_time );

	std::string logFilename;
	GG::StringHelper::Format(
		logFilename,
		"ram://logs/log_%d-%d-%d_%dh-%dm-%ds.txt",
		t->tm_mon + 1,
		t->tm_hour + 1,
		t->tm_year + 1900,
		t->tm_hour,
		t->tm_min,
		t->tm_sec
		);

	return logFilename;
}
