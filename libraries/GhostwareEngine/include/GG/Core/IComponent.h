// Ghostware Games inc. 2016  -Julian Williams
#pragma once

#include <GG/Core/Types.h>

namespace GG
{
	class Entity;

	class IComponent
	{
	public:
		IComponent() {}
		virtual ~IComponent() {}

		virtual void setEntity( Entity * entity )
		{
			_entity = entity;
		}

		const Entity * getEntity() const
		{
			return _entity;
		}

		Entity * getEntity()
		{
			return _entity;
		}

	protected:
		Entity * _entity;
	};

}
