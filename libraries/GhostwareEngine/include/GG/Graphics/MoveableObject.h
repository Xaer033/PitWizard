// Ghostware Games inc. 2016  -Julian Williams
#pragma once

#include "SceneNode.h"

namespace GG
{

	class MoveableObject
	{
		friend SceneNode;
	public:
		MoveableObject() : _sceneNode( nullptr ) {}
		virtual ~MoveableObject() {}

		SceneNode *			getSceneNode()			{ return _sceneNode; }
		const SceneNode *	getSceneNode() const	{ return _sceneNode; }
	private:
		SceneNode *	_sceneNode;
	};
}