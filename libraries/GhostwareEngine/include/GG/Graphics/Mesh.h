// Ghostware Games inc. 2016  -Julian Williams
#pragma once

#include "RenderableObject.h"
#include "RenderFactory.h"
#include "Material.h"
#include "Model.h"
#include <GG/Core/StringId.h>

#include <GG/EntitySystem/Entity.h>

namespace GG
{
	class Mesh : 
		public IRenderableObject, 
		public IComponent
	{
	public:

		static const uint32 GetComponentId()
		{
			static StringId id = STRING_ID( "MeshComponent" );
			return id;
		}

	public:

		Mesh();
		Mesh( Model * geo, Material * mat );
		virtual ~Mesh();

		virtual void render( RenderFactory & renderFactory );

		Model *			geometry;
		Material *	material;
	};
}