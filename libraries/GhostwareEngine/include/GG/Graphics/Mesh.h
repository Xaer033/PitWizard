// Ghostware Games inc. 2016  -Julian Williams
#pragma once

#include "RenderableObject.h"
#include "RenderFactory.h"
#include "Material.h"
#include "Model.h"
#include "IwHashString.h"
#include <GG/EntitySystem/Entity.h>

namespace GG
{
	class Mesh : 
		public RenderableObject, 
		public IComponent
	{
	public:

		static const uint32 GetComponentId()
		{
			static uint32 id = IwHashString( "MeshComponent" );
			return id;
		}

	public:

		Mesh();
		Mesh( Model * geo, Material * mat );
		virtual ~Mesh();

		virtual void render( RenderFactory & renderFactory );

		Model *		geometry;
		Material *	material;
	};
}