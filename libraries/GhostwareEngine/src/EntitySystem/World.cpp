
#include "World.h"

#include <GG/Graphics/Mesh.h>
#include <GG/Graphics/Camera.h>
#include <GG/Core/Log.h>

namespace GG
{
	uint World::kMaxEntities = 3000;

	World::World( ISceneGraph * graph ) :
		_renderSystem( new RenderSystem( graph ) )
	{
		_entityList = new Entity[ kMaxEntities ];
	}

	World::~World()
	{
		if( _entityList != nullptr )
		{
			delete [] _entityList;
			_entityList = nullptr;
		}
	}

	Entity * World::createEntity( const std::string & name )
	{
		static uint entityCount = 0;
		if( entityCount == kMaxEntities )
		{
			LOG_WARNING( "Max Entity count: %d reached!", kMaxEntities );
			return nullptr;
		}
		SceneNode * node = createSceneNode();
		_entityList[entityCount] = Entity( entityCount, name, node );
		return &_entityList[ entityCount++ ];
	}

	SceneNode * World::createSceneNode( )
	{
		return _renderSystem->createSceneNode( );
	}

	Camera * World::createCamera( const std::string & name )
	{
		Entity *	camEntity	= createEntity( name );
		if( !Entity::IsValid(camEntity)) 
			return nullptr;

		Camera *	camera		= addComponent<Camera>( camEntity );
		if( camera == nullptr )
			return nullptr;

		return camera;
	}


	void World::update( float deltaTime )
	{

	}

	void World::renderOneFrame()
	{
		_renderSystem->renderOneFrame();
	}


//------------------------------------------------------------
	IComponent * World::_addComponentFromType( 
		Entity * entity, 
		uint32 componentType )
	{
		if( componentType == Mesh::GetComponentId() )
		{
			Mesh * mesh = _renderSystem->addMeshComponent( entity->getId() );
			entity->getSceneNode()->attachObject( mesh );
			return mesh;
		}

		if( componentType == Camera::GetComponentId() )
			return _renderSystem->addCamera( entity->getId() );

		return nullptr;
	}

	IComponent * World::_getComponentFromType(
		const Entity * entity,
		uint32 componentType )
	{
		if( componentType == Mesh::GetComponentId() )
			return _renderSystem->getMeshComponent( entity->getId() );

		if( componentType == Camera::GetComponentId() )
			return _renderSystem->getCamera( entity->getId() );

		return nullptr;
	}
}