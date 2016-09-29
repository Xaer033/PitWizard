
#include "PitWizard.h"

#include <iostream>
#include <fstream>
#include <chrono>


#include "s3e.h"

//#include <Photon-cpp/inc/PhotonPeer.h>

#include <GG/GameLayer.h>
#include <GG/Graphics.h>
#include <GG/Core.h>
#include <GG/Input.h>
#include <GG/EntitySystem.h>

using namespace GG;


PitWizard::PitWizard() : _deviceWidth(0), _deviceHeight(0)
{

}

PitWizard::~PitWizard()
{

}

void PitWizard::init()
{
	_setupLoggers();
	Application::init();

	_deviceWidth	= IwGLGetInt(IW_GL_WIDTH);
	_deviceHeight	= IwGLGetInt(IW_GL_HEIGHT);

	_gameStateManager.addGameState( "Intro",					nullptr );
	_gameStateManager.addGameState( "MainMenu",					nullptr );
	_gameStateManager.addGameState( "SinglePlayer",				nullptr );
	_gameStateManager.addGameState( "SinglePlayerEndScreen",	nullptr );
	_gameStateManager.addGameState( "MultiPlayer",				nullptr );
	_gameStateManager.addGameState( "MultiPlayerEndScreen",		nullptr );


	CIwResGroup * group = IwGetResManager()->LoadGroup("./resources/creeps/creep_02.group");
	_test		= (CIwModel*)group->GetResNamed("Cube", IW_GRAPHICS_RESTYPE_MODEL);

	json config = JsonFromFile("configs/input_config.json");
	inputSystem = new InputSystem(IwGxGetScreenWidth(), IwGxGetScreenHeight());
	//inputSystem = new InputSystem(_deviceWidth, _deviceHeight);
	inputSystem->init(config);
}

void PitWizard::shutdown()
{
	inputSystem->shutdown();
	delete(inputSystem);

	Log::Shutdown();
	Application::shutdown();
}


std::string loadFile(const std::string & filename)
{
	std::ifstream in(filename, std::ios::in | std::ios::binary);

	std::string contents;

	if(in)
	{
		in.seekg(0, std::ios::end);
		contents.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&contents[0], contents.size());
		in.close();
	}

	return contents;
}

void PitWizard::doGameLoop()
{
	Texture * brickTexture = new Texture();
	brickTexture->LoadFromFile("./resources/environments/forest_road/textures/brick_albedo.png");
	brickTexture->Upload();

	CIwResGroup * group = IwGetResManager()->LoadGroup("./resources/environments/forest_road/staticGeometry.group");
	Model * gridModel	= (Model*)group->GetResNamed("grid", IW_GRAPHICS_RESTYPE_MODEL);
	Material * gridMat = IW_GX_ALLOC_MATERIAL();
	gridMat->SetModulateMode(CIwMaterial::MODULATE_NONE);
	gridMat->SetTexture(brickTexture);

	Model * groundModel		= (Model*)group->GetResNamed("groundPlane", IW_GRAPHICS_RESTYPE_MODEL);
	Material * groundMat	= (Material*)group->GetResNamed("groundPlane/groundMat", IW_GX_RESTYPE_MATERIAL);
	groundMat->SetAlphaMode(CIwMaterial::AlphaMode::ALPHA_NONE);
	groundMat->SetTexture(brickTexture, 0);

	World* worldPtr = new World(new BasicSceneGraph(1000));
	std::unique_ptr<World> world(worldPtr);

	float aspect =	(float)IwGxGetScreenWidth() /
		(float)IwGxGetScreenHeight();

	GG::Entity *	box_1		= world->createEntity("Box_1");
	GG::Mesh *		boxMesh_1	= world->addComponent<Mesh>(box_1);
	boxMesh_1->geometry = _test;
	SceneNode *		boxNode_1	= box_1->getSceneNode();
	boxNode_1->setPosition(Vector3(0, -3, 0));

	GG::Entity *	box_2		= world->createEntity("Box_2");
	GG::Mesh *		boxMesh_2	= world->addComponent<Mesh>(box_2);
	boxMesh_2->geometry = _test;
	SceneNode *		boxNode_2	= box_2->getSceneNode();
	boxNode_2->setPosition(Vector3(0, -2, 3));
	boxNode_2->setParent(boxNode_1);

	GG::Camera * cam = world->createCamera("Camera_1");
	cam->setPerspective(60.0f, aspect, 0.1f, 300.0f);
	cam->setViewport(0, 0, 1, 1);
	cam->setClearMode(Camera::ClearMode::Depth | Camera::ClearMode::Color);
	cam->setClearColor(GG::Vector4(0.1f, 0.03f, 0.14f, 1));
	GG::SceneNode * camNode_1 = cam->getEntity()->getSceneNode();
	camNode_1->setPosition(Vector3(0, 4, 10));


	Entity *	gridEntity = world->createEntity("GridMesh");
	Mesh *		gridMeshInstance	= world->addComponent<Mesh>(gridEntity);
	gridMeshInstance->geometry		= gridModel;
	gridMeshInstance->material		= gridMat;


	Entity *	groundEntity = world->createEntity("GroundMesh");
	Mesh *		groundMeshInstance	= world->addComponent<Mesh>(groundEntity);
	groundMeshInstance->geometry	= groundModel;
	groundMeshInstance->material	= gridMat;

	// Loop forever, until the user or the OS performs some action to quit the app
	while(!s3eDeviceCheckQuitRequest())
	{
		//Update the input systems
		inputSystem->update();

		if(s3eKeyboardGetState(s3eKeyLeft) & S3E_KEY_STATE_DOWN)
		{
			boxNode_1->rotate(0.04f, Vector::up());
		}
		else if(s3eKeyboardGetState(s3eKeyRight) & S3E_KEY_STATE_DOWN)
		{
			boxNode_1->rotate(-0.04f, Vector::up());
		}

		if(s3eKeyboardGetState(s3eKeyUp) & S3E_KEY_STATE_DOWN)
		{
			boxNode_1->translate(boxNode_1->forward() * 0.1f);
		}
		else if(s3eKeyboardGetState(s3eKeyDown) & S3E_KEY_STATE_DOWN)
		{
			boxNode_1->translate(boxNode_1->forward() * -0.1f);
		}
		if(s3eKeyboardGetState(s3eKeyP) & S3E_KEY_STATE_RELEASED)
		{
			bool hasParent = boxNode_2->getParent() != nullptr;
			boxNode_2->setParent(hasParent ? nullptr : boxNode_1);
		}
		camNode_1->rotate( 1.4f * inputSystem->getAxis( "RotateX" ), -Vector::up() );

		float walk		= inputSystem->getAxis( "Walk" );
		float strafe	= inputSystem->getAxis( "Strafe" );

		Vector3 forwardYLock = camNode_1->forward();
		forwardYLock.y = 0;

		Vector3 move =	(forwardYLock * walk) +
						(camNode_1->right() * strafe);

		move = glm::length2(move) > 0.001f ? glm::normalize(move): move;
		camNode_1->translate(move * 0.4f);

		world->update(1.0f / 60.0f);
		
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
