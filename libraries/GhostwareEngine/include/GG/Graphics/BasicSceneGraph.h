// Ghostware Games inc. 2016  -Julian Williams
#pragma once

#include <string>
#include <array>
#include "ISceneGraph.h"

namespace GG
{
	class BasicSceneGraph : public ISceneGraph
	{
	public:
		BasicSceneGraph( int maxSize );
		virtual ~BasicSceneGraph();

		virtual void			init();
		virtual void			shutdown();

		virtual SceneNode *		createSceneNode( );
		virtual void			removeSceneNode( SceneNode * node );

		virtual bool			getVisibleObjects( const Camera * cam, RenderableObjectList & renderList );
	
	private:
		std::vector<SceneNode> _sceneNodeList;
	};
}