
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

#include <GG/Graphics/MaterialSerializer.h>

#include <GG/Resources/ResourceManager.h>
#include <GG/Resources/IResourceDescriptor.h>

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


	_test = nullptr;


	json config = JsonFromFile("configs/input_config.json");
	inputSystem = std::unique_ptr<InputSystem>(new InputSystem(_deviceWidth, _deviceHeight));
	inputSystem->init(config);

	RenderState::GetInstance()->setRenderSize(_deviceWidth, _deviceHeight);

	PHYSFS_init(nullptr);

	const char* error = PHYSFS_getLastError();
	PHYSFS_mount("rom://", NULL, 1);
	PHYSFS_mount("ram://", NULL, 1);
	PHYSFS_mount("/", NULL, 1);
	PHYSFS_setWriteDir("ram://");

	const PHYSFS_ArchiveInfo **i = PHYSFS_supportedArchiveTypes();
	for(; *i != NULL; i++)
	{
		LOG_INFO("INFO: %s, %s", (*i)->extension, (*i)->description);
	}

	ResourceManager::GetInstance()->registerType<Texture2D>();
	FileStream stream("resources/TestGroup.group", OpenMode::OPEN_READ);
	ResourceGroup* group = ResourceManager::GetInstance()->createGroup(&stream);
	group->loadAllAssets();
}

void PitWizard::shutdown()
{
	PHYSFS_deinit();
	inputSystem->shutdown();

	Log::Shutdown();
	IwGLTerminate();
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
	Material * gridMat = new Material();
	std::string jsonMat = loadFile("ram://mat.txt");
	MaterialSerializer matSerializer;
	//matSerializer.serialize(*gridMat, jsonMat);
	matSerializer.deserialize(*gridMat, jsonMat);

	/*s3eFile * _fileHandle = s3eFileOpen("ram://mat.txt", "w");

	s3eFileWrite(
		jsonMat.data(),
		sizeof(char),
		jsonMat.size() * sizeof(char),
		_fileHandle
	);

	s3eFileFlush(_fileHandle);
*/
	Model gridModel;
	gridModel.setVertexProperties(POSITIONS | TEXCOORDS | NORMALS | TANGENTS | BITANGENTS);
	ObjLoader::loadFromFile("./resources/environments/forest_road/models/grid.obj", gridModel);

	Model groundModel;
	groundModel.setVertexProperties(POSITIONS | TEXCOORDS | NORMALS | TANGENTS | BITANGENTS);
	ObjLoader::loadFromFile("./resources/environments/forest_road/models/groundPlane.obj", groundModel);
	
	Model playerModel;
	playerModel.setVertexProperties(POSITIONS | TEXCOORDS | NORMALS | TANGENTS | BITANGENTS);
	ObjLoader::loadFromFile("./resources/environments/forest_road/models/player.obj", playerModel);

	World* worldPtr = new World(new BasicSceneGraph(1000));
	std::unique_ptr<World> world(worldPtr);

	float aspect =	(float)_deviceWidth / (float)_deviceHeight;

	GG::Entity *	box_1		= world->createEntity("Box_1");
	GG::Mesh *		boxMesh_1	= world->addComponent<Mesh>(box_1);
	boxMesh_1->geometry = &playerModel;
	boxMesh_1->material	= gridMat;
	SceneNode *		boxNode_1	= box_1->getSceneNode();
	boxNode_1->setPosition(Vector3(0, 1, 0));

	GG::Entity *	box_2		= world->createEntity("Box_2");
	GG::Mesh *		boxMesh_2	= world->addComponent<Mesh>(box_2);
	boxMesh_2->geometry = &playerModel;
	boxMesh_2->material	= gridMat;
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
	camNode_1->lookAt(boxNode_1);

	Entity *	gridEntity = world->createEntity("GridMesh");
	Mesh *		gridMeshInstance	= world->addComponent<Mesh>(gridEntity);
	gridMeshInstance->geometry		= &gridModel;
	gridMeshInstance->material		= gridMat;


	Entity *	groundEntity = world->createEntity("GroundMesh");
	Mesh *		groundMeshInstance	= world->addComponent<Mesh>(groundEntity);
	groundMeshInstance->geometry	= &groundModel;
	groundMeshInstance->material	= gridMat;
	
	const float camSpeed	= 0.035f;

	// Loop forever, until the user or the OS performs some action to quit the app
	while(!s3eDeviceCheckQuitRequest())
	{
		//Update the input systems
		inputSystem->update();

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
			camNode_1->translate(Vector::down() * camSpeed);
		}
		else if(s3eKeyboardGetState(s3eKeyX) & S3E_KEY_STATE_DOWN)
		{
			camNode_1->translate(Vector::up() * camSpeed);
		}
		camNode_1->rotate(Angle::FromDegrees(70.0f) * inputSystem->getAxis( "RotateX" ), Vector::right() );

		float walk		= inputSystem->getAxis( "Walk" );
		float strafe	= inputSystem->getAxis( "Strafe" );

		Vector3 move =	(Vector::forward() * walk) +
						(camNode_1->right() * strafe);

		move = glm::length2(move) > 0.001f ? glm::normalize(move): move;
		move.y = 0;
		camNode_1->translate(move * camSpeed);

		world->update(1.0f / 60.0f);
		
		world->renderOneFrame();

			// Sleep for 0ms to allow the OS to process events etc.
		s3eDeviceYield( 0 );
	}
	delete gridMat;
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
