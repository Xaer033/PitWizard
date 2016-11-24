
#include "PitWizard.h"

#include <iostream>
#include <fstream>
#include <chrono>

#include <Physfs/physfs.h>

#include "s3e.h"

//#include <Photon-cpp/inc/PhotonPeer.h>

#include <GG/GameLayer.h>
#include <GG/Graphics.h>
#include <GG/Core.h>
#include <GG/Input.h>
#include <GG/EntitySystem.h>

#include <GG/Resources/ResourceManager.h>

using namespace GG;


PitWizard::PitWizard() : _deviceWidth(0), _deviceHeight(0)
{

}

PitWizard::~PitWizard()
{

}

void PitWizard::init(int argc, char** argv)
{
	_setupLoggers();

	const char * argv0 = (argc > 0) ? argv[0] : nullptr;
	FileSystem::Init(argv0);
	FileSystem::Mount("rom://", "/", true);
	FileSystem::Mount("ram://", "/", true);
	FileSystem::Mount("/", "/", true);
	FileSystem::SetWriteDirectory("ram://");

	s3eSurfaceSetup(S3E_SURFACE_PIXEL_TYPE_RGB888);

	//Initialise graphics system(s)
	if(!IwGLInit())
	{
		TRACE_ERROR("IwGLInit failed");
		return;
	}

	IwGLSwapBuffers();
	s3eDeviceYield(0);

	_deviceWidth	= IwGLGetInt(IW_GL_WIDTH);
	_deviceHeight	= IwGLGetInt(IW_GL_HEIGHT);

	_gameStateManager.addGameState( "Intro",					nullptr );
	_gameStateManager.addGameState( "MainMenu",					nullptr );
	_gameStateManager.addGameState( "SinglePlayer",				nullptr );
	_gameStateManager.addGameState( "SinglePlayerEndScreen",	nullptr );
	_gameStateManager.addGameState( "MultiPlayer",				nullptr );
	_gameStateManager.addGameState( "MultiPlayerEndScreen",		nullptr );

	json config = JsonFromFile("configs/input_config.json");
	inputSystem = std::unique_ptr<InputSystem>(new InputSystem(_deviceWidth, _deviceHeight));
	inputSystem->init(config);

	RenderState::Get()->setRenderSize(_deviceWidth, _deviceHeight);

	ResourceManager::Get()->registerType<Texture2D>(new Texture2DLoader());
	ResourceManager::Get()->registerType<Shader>(new ShaderLoader());
	ResourceManager::Get()->registerType<Material>(new MaterialLoader());

	ResourceGroup* group = ResourceManager::Get()->createGroupFromPack("resources/Cool.group");
	group->loadAllAssets();
}

void PitWizard::shutdown()
{
	inputSystem->shutdown();

	ResourceManager::ShutDown();

	Log::Shutdown();
	IwGLTerminate();
	FileSystem::Shutdown();
}


void PitWizard::doGameLoop()
{
	ResourceManager * rm = ResourceManager::Get();
	ResourceH<Material> gridMat = rm->getResource<Material>("marble");

	Mesh gridModel;
	FileStream gStream("resources/gridFloor.gmesh", OpenMode::OPEN_READ);
	GmeshLoader::LoadFromStream(&gStream, &gridModel);

	
	Mesh playerModel;
	FileStream pStream("resources/player.gmesh", OpenMode::OPEN_READ);
	GmeshLoader::LoadFromStream(&pStream, &playerModel);

	ResourceHandle<Material> tempHolder = rm->getResource<Material>("marble");

	World* worldPtr = new World(new BasicSceneGraph(1000));
	std::unique_ptr<World> world(worldPtr);

	float aspect =	(float)_deviceWidth / (float)_deviceHeight;

	GG::Entity *		box_1		= world->createEntity("Box_1");
	GG::MeshInstance *	boxMesh_1	= world->addComponent<MeshInstance>(box_1);
	boxMesh_1->setMesh(&playerModel);
	boxMesh_1->mainMaterial(STRING_ID("marble"));
	SceneNode *		boxNode_1	= box_1->getSceneNode();
	boxNode_1->setPosition(Vector3(0, 1, 0));

	GG::Entity *		box_2		= world->createEntity("Box_2");
	GG::MeshInstance *	boxMesh_2	= world->addComponent<MeshInstance>(box_2);
	boxMesh_2->setMesh(&playerModel);
	boxMesh_2->mainMaterial(STRING_ID("marble"));
	SceneNode *		boxNode_2	= box_2->getSceneNode();
	boxNode_2->setPosition(Vector3(0, 0, 3));
	boxNode_2->setParent(boxNode_1);

	GG::Camera * cam = world->createCamera("Camera_1");
	cam->setPerspective(Angle::FromDegrees(80.0f), aspect, 0.1f, 300.0f);
	cam->setViewport(0, 0, 1, 1);
	cam->setClearMode(RenderState::ClearMode::CM_DEPTH | RenderState::ClearMode::CM_COLOR);
	cam->setClearColor(GG::Vector4(0.43f, 0.48f, 0.67f, 1.0f));
	GG::SceneNode * camNode_1 = cam->getEntity()->getSceneNode();
	camNode_1->setPosition(Vector3(0.0f, 3.0f, -7.0f));
	camNode_1->lookAt(Vector3());

	Entity *		groundEntity		= world->createEntity("GroundMesh");
	MeshInstance *	groundMeshInstance	= world->addComponent<MeshInstance>(groundEntity);
	groundMeshInstance->setMesh(&gridModel);
	groundMeshInstance->mainMaterial(STRING_ID("marble"), true);

	const float camSpeed	= 5.0f;

	Time clock;

	// Loop forever, until the user or the OS performs some action to quit the app
	while(!s3eDeviceCheckQuitRequest())
	{
		clock.update();
		float deltaTime = clock.getDeltaTime();
		
		//Update the input systems
		inputSystem->update();
		// TEMP RESOURCE RELOADING!!! -Julian
		if(s3eKeyboardGetState(s3eKeyR) & S3E_KEY_STATE_RELEASED)
		{
			/*ResourceHandle<Shader> shader = rm->getResource<Shader>(STRING_ID("cookTorrence"));
			shader->setState(IResource::State::UNLOADED);
*/
			rm->findGroup(STRING_ID("default"))->reloadAllAssets();
		}

		if(s3eKeyboardGetState(s3eKeyLeft) & S3E_KEY_STATE_DOWN)
		{
			boxNode_1->rotate(Angle::FromDegrees(3.0f), Vector::up());
		}
		else if(s3eKeyboardGetState(s3eKeyRight) & S3E_KEY_STATE_DOWN)
		{
			boxNode_1->rotate(Angle::FromDegrees(-3.0f), Vector::up());
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

		if(s3eKeyboardGetState(s3eKeyZ) & S3E_KEY_STATE_DOWN)
		{
			camNode_1->translate(Vector::down() * camSpeed * deltaTime);
		}
		else if(s3eKeyboardGetState(s3eKeyX) & S3E_KEY_STATE_DOWN)
		{
			camNode_1->translate(Vector::up() * camSpeed * deltaTime);
		}
		camNode_1->rotate(Angle::FromDegrees(70.0f) * inputSystem->getAxis( "RotateX" ), Vector::right() );

		float walk		= inputSystem->getAxis( "Walk" );
		float strafe	= inputSystem->getAxis( "Strafe" );

		Vector3 move =	(Vector::forward() * walk) +
						(camNode_1->right() * strafe);

		move = glm::length2(move) > 0.001f ? glm::normalize(move): move;
		camNode_1->translate(move * camSpeed * deltaTime);

		world->update(1.0f / 60.0f);
		
		world->renderOneFrame();

			// Sleep for 0ms to allow the OS to process events etc.
		s3eDeviceYield( 0 );
	}
}

void PitWizard::_setupLoggers()
{
	const std::string filePath = _getFileLogName();

	Log::Get()->registerLogger( new FileLogger( filePath ) );
	Log::Get()->registerLogger( new ConsoleLogger() );
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
