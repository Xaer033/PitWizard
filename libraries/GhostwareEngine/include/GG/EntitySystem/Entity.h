// Ghostware Games inc. 2016  -Julian Williams
#pragma once

#include <string>
#include <GG/Core/ObjectId.h>
#include <GG/Graphics/SceneNode.h>

namespace GG
{
	class World;
	class Entity
	{
		friend World;
	public:
		
		static bool IsValid( const Entity * entity )
		{
			return entity != nullptr && entity->getId() >= 0;
		}

		const std::string & getName() const;
		void				setName( const std::string & name );

		const ObjectId		getId() const;

		const SceneNode *	getSceneNode() const;
		SceneNode *			getSceneNode();


	private:
		Entity();
		Entity(
			const ObjectId & id,
			const std::string & name,
			SceneNode * node
		);

		SceneNode *		_sceneNode;
		std::string		_name;
		ObjectId		_id;
	};
}

