// Ghostware Games inc. 2016  -Julian Williams
#pragma once

#include "RenderFactory.h"
#include "ISceneGraph.h"
#include <GG/Core/HashString.h>

#include "ObjectId.h"
#include "Mesh.h"

#include <unordered_map>

namespace GG
{
	class RenderSystem
	{
	public:
		static uint32 GetSystemId()
		{
			static uint32 systemId = hashString( "RenderSystem" );
			return systemId;
		}

	public:
		RenderSystem( ISceneGraph * graph );
		~RenderSystem();

		SceneNode * createSceneNode();

		Mesh *		addMeshComponent( const ObjectId & id );
		Mesh *		getMeshComponent( const ObjectId & id );

		Camera *	addCamera( const ObjectId & id );
		Camera *	getCamera( const ObjectId & id );

		void		renderOneFrame();

	private:
		typedef std::unordered_map< ObjectId, Mesh>		MeshMap;
		typedef std::unordered_map< ObjectId, Camera>	CameraMap;

		std::unique_ptr<ISceneGraph>	_graph;

		CameraMap				_cameraMap;
		MeshMap					_meshMap;
		RenderFactory			_renderFactory;
		RenderableObjectList	_renderableList;
	};
}