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

	SceneNode* BasicSceneGraph::createSceneNode( )
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

	bool BasicSceneGraph::getVisibleObjects( const Camera * cam, RenderableObjectList & renderList )
	{
		for( uint i = 0; i < _sceneNodeList.size(); ++i )
		{
			RenderableObject* renderable = _sceneNodeList[ i ].getObject();
			if( renderable != nullptr )
			{
				renderList.push_back( renderable );
			}
		}
		return true;
	}
}
