// Ghostware Games inc. 2016  -Julian Williams
#pragma once

#include <array>
#include <set>

#include "Entity.h"

#include <GG/Graphics/Camera.h>
#include <GG/Graphics/ISceneGraph.h>
#include <GG/Graphics/RenderSystem.h>
#include <GG/Core/Log.h>

namespace GG
{
	class World
	{
	public:
		static uint kMaxEntities;

	public:
		World( ISceneGraph * graph );
		~World();

		Entity *		createEntity( const std::string & name );
		SceneNode *		createSceneNode();
		Camera *		createCamera( const std::string & name );
		

		void			update( float deltaTime );
		void			renderOneFrame();


		template <typename T, typename... Args>
		T*	addComponent( Entity * entity, Args &&... args )
		{
			if( entity == nullptr )
			{
				LOG_ERROR( "Entity you are trying to add a component to is null!" );
				return nullptr;
			}

			IComponent * component = _addComponentFromType(
				entity, 
				T::GetComponentType()
			);

			if( component == nullptr )
			{
				LOG_ERROR( "Component for entity %s did was not created properly!", entity->getName() );
				return nullptr;
			}

			component->setEntity( entity );
			return static_cast<T*>( component );
		}
	
		template <typename T>
		T*	getComponent( Entity * entity )
		{
			if( entity == nullptr )
			{
				LOG_ERROR( "Entity you are trying to get a component from is null!" );
				return nullptr;
			}

			IComponent * component = _getComponentFromType( 
				entity, 
				T::GetComponentType() 
			);

			return static_cast<T*>( component );
		}

	private:
		IComponent * _addComponentFromType( Entity * entity, uint32 componentType );
		IComponent * _getComponentFromType( const Entity * entity, uint32 componentType );

	private:
		std::unique_ptr<RenderSystem>		_renderSystem;
		Entity *							_entityList;

	};
}