
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
#include <GG/EntitySystem.h>

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
	World* worldPtr = new World( new BasicSceneGraph( 1000 ) );
	std::unique_ptr<World> world( worldPtr );

	GG::SceneNode boxTransform;

	float aspect =	( float )IwGxGetScreenWidth() / 
					( float )IwGxGetScreenHeight();
	
	GG::Camera * cam = world->createCamera( "Camera_1" );
	GG::SceneNode * camNode_1 = cam->getEntity()->getSceneNode();
	camNode_1->setPosition( GG::Vector3( 2, 5, 10 ) );
	camNode_1->lookAt( &boxTransform );
	cam->setPerspective( 60.0f, aspect, 0.1f, 300.0f );
	cam->setViewport( 0, 0, 1, 1 );
	cam->setClearMode( GG::ClearMode::Depth | GG::ClearMode::Color );
	cam->setClearColor( GG::Vector4( 0.1f, 0.03f, 0.14f, 1 ) );



	float _angle = 0;
	GG::SceneNode parent;
	parent.setPosition( GG::Vector3( -4.0f, 0, 0 ) );

	GG::Entity *	box_1		= world->createEntity( "Box_1" );
	GG::Mesh *		boxMesh_1	= world->addComponent<Mesh>( box_1 );
	boxMesh_1->geometry = _test;
	GG::SceneNode * boxNode_1	= box_1->getSceneNode();
	boxNode_1->setParent( &parent );

	// Loop forever, until the user or the OS performs some action to quit the app
	while( !s3eDeviceCheckQuitRequest() )
	{
		//Update the input systems
		inputSystem->update();
		
		camNode_1->rotate( inputSystem->getAxis( "RotateX" ), -Vector3::g_AxisY );

		float walk		= inputSystem->getAxis( "Walk" );
		float strafe	= inputSystem->getAxis( "Strafe" );

		Vector3 move =	 ( camNode_1->forward() * walk ) + 
						( camNode_1->right() * strafe );

		move = move.GetLengthSquared() > 0.001f ? move.GetNormalised() : move;
		camNode_1->translate( move * 0.4f);

		world->update( 1.0f / 60.0f );
		
		world->renderOneFrame();


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
