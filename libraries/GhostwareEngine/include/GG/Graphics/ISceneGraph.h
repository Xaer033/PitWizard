// Ghostware Games inc. 2016  -Julian Williams
#pragma once
#include <string>
#include <vector>
#include "MoveableObject.h"
#include "SceneNode.h"
#include "Camera.h"

namespace GG
{

	typedef std::vector<MoveableObject*> MoveableObjectList;
	class ISceneGraph
	{
	public:
		ISceneGraph() {}
		virtual ~ISceneGraph() {}

		virtual void			init() = 0;
		virtual void			shutdown() = 0;

		virtual SceneNode *		createSceneNode( const std::string & name = "node" ) = 0;
		virtual void			removeSceneNode( SceneNode * node ) = 0;

		virtual bool			getVisibleObjects( const Camera * cam, MoveableObjectList & moveableObjectList ) = 0;
	};
}