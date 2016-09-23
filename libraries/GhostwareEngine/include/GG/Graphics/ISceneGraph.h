// Ghostware Games inc. 2016  -Julian Williams
#pragma once
#include <string>
#include <vector>

#include "RenderableObject.h"
#include "SceneNode.h"
#include "Camera.h"

namespace GG
{
	typedef std::vector<RenderableObject*> RenderableObjectList;
	class ISceneGraph
	{
	public:
		ISceneGraph() {}
		virtual ~ISceneGraph() {}

		virtual void			init() = 0;
		virtual void			shutdown() = 0;

		virtual SceneNode *		createSceneNode() = 0;
		virtual void			removeSceneNode( SceneNode * node ) = 0;

		virtual bool			getVisibleObjects( const Camera * cam, RenderableObjectList & renderList ) = 0;
	};
}