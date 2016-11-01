#include "RenderSystem.h"

#include <unordered_map>
#include <s3e.h> //Temp

#include <GG/Core/Log.h>
#include <GG/Core/StringId.h>
#include <GG/Resources/ResourceManager.h>

#include "RenderFactory.h"
#include "ISceneGraph.h"
#include "Mesh.h"

namespace GG
{
	RenderSystem::RenderSystem( ISceneGraph * graph ) : 
		_graph(graph)
	{
	}

	RenderSystem::~RenderSystem()
	{
		_cameraMap.clear();
		_renderableList.clear();
	}

	SceneNode * RenderSystem::createSceneNode()
	{
		if( _graph == nullptr )
		{
			TRACE_ERROR( "Render System does not have a valid scene graph!" );
			return nullptr;
		}
		return _graph->createSceneNode( );
	}

	Mesh * RenderSystem::addMeshComponent( const StringId & id )
	{
		_meshMap[ id ] = Mesh(nullptr, nullptr);
		return &_meshMap[ id ];
	}

	Mesh * RenderSystem::getMeshComponent( const StringId & id )
	{
		auto iter = _meshMap.find( id );
		if( iter == _meshMap.end() )
		{
			TRACE_WARNING( "Could not find mesh with id: %d", id );
			return nullptr;
		}
		return &iter->second;
	}

	Camera * RenderSystem::addCamera( const StringId & id )
	{
		_cameraMap[ id ] = Camera();
		return &_cameraMap[ id ];
	}

	Camera * RenderSystem::getCamera( const StringId & id )
	{
		auto iter = _cameraMap.find( id );
		if( iter == _cameraMap.end() )
		{
			TRACE_WARNING( "Could not find camera with id: %d", id );
			return nullptr;
		}
		return &iter->second;
	}

	void RenderSystem::renderOneFrame()
	{
		auto it = _cameraMap.begin();
		for(; it != _cameraMap.end(); ++it )
		{
			_renderableList.clear();
			_graph->getVisibleObjects(&it->second, _renderableList );

			for( uint i = 0; i < _renderableList.size(); ++i )
			{
				IRenderableObject * renderable = _renderableList[ i ];
				if( renderable == nullptr ) continue;

				renderable->render( _renderFactory );
			}

			_renderFactory.renderAll( &it->second );
			_renderFactory.clearAllCommands();
		}

		IwGLSwapBuffers();
	}
}