#include "BasicSceneGraph.h"

namespace GG
{
	BasicSceneGraph::BasicSceneGraph( int maxSize )
	{
		_sceneNodeList.reserve( maxSize );
	}
	BasicSceneGraph::~BasicSceneGraph()
	{
	}

	void BasicSceneGraph::init()
	{
	}

	void BasicSceneGraph::shutdown()
	{
		_sceneNodeList.clear();
	}

	SceneNode* BasicSceneGraph::createSceneNode( const std::string& name )
	{
		_sceneNodeList.emplace_back( SceneNode( ) );
		return &_sceneNodeList.back();
	}

	void BasicSceneGraph::removeSceneNode( SceneNode * node )
	{
		for( uint i = 0; i < _sceneNodeList.size(); ++i )
		{
			if( &_sceneNodeList[i] == node )
			{
				_sceneNodeList.erase( _sceneNodeList.begin() + i );
				return;
			}
		}
	}

	bool BasicSceneGraph::getVisibleObjects( const Camera * cam, MoveableObjectList & renderList )
	{
		for( uint i = 0; i < _sceneNodeList.size(); ++i )
		{
			MoveableObject* moveable = _sceneNodeList[ i ].getObject();
			if( moveable != nullptr )
			{
				renderList.push_back( moveable );
			}
		}

		return true;
	}
}
