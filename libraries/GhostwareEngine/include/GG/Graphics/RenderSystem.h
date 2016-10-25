// Ghostware Games inc. 2016  -Julian Williams
#pragma once

#include "RenderFactory.h"
#include "ISceneGraph.h"
#include <GG/Core/StringId.h>

#include "StringId.h"
#include "Mesh.h"

#include <unordered_map>

namespace GG
{
	class RenderSystem
	{
	public:
		static uint32 GetSystemId()
		{
			static StringId systemId = STRING_ID( "RenderSystem" );
			return systemId;
		}

	public:
		RenderSystem( ISceneGraph * graph );
		~RenderSystem();

		SceneNode * createSceneNode();

		Mesh *		addMeshComponent( const StringId & id );
		Mesh *		getMeshComponent( const StringId & id );

		Camera *	addCamera( const StringId & id );
		Camera *	getCamera( const StringId & id );

		void		renderOneFrame();

	private:
		typedef std::unordered_map< StringId, Mesh>		MeshMap;
		typedef std::unordered_map< StringId, Camera>	CameraMap;

		std::unique_ptr<ISceneGraph>	_graph;

		CameraMap				_cameraMap;
		MeshMap					_meshMap;
		RenderFactory			_renderFactory;
		RenderableObjectList	_renderableList;
	};
}