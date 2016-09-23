// Ghostware Games inc. 2016  -Julian Williams
#include "Entity.h"
#include <GG/Core/Log.h>

namespace GG
{
	Entity::Entity() :
		_id( -1 )
	{
	}

	Entity::Entity( 
		const ObjectId & id, 
		const std::string & name, 
		SceneNode * node ) :
			_id( id ),
			_name( name )
	{

		_sceneNode = node;
		if( _sceneNode == nullptr )
		{
			LOG_ERROR( "Node in Entity constructor is null!" );
			return;
		}
	}

	const std::string & Entity::getName() const 
	{
		return _name;
	}

	void Entity::setName( const std::string & name ) 
	{
		_name = name;
	}

	const ObjectId Entity::getId() const
	{
		return _id;
	}

	const SceneNode * Entity::getSceneNode() const
	{
		return _sceneNode;
	}

	SceneNode * Entity::getSceneNode()
	{
		return _sceneNode;
	}

}