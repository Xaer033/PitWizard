// Ghostware Games inc. 2016  -Julian Williams
#pragma once

#include "MoveableObject.h"
#include "Material.h"
#include "Model.h"

namespace GG
{
	class Mesh : public MoveableObject
	{
	public:
		Mesh( Model * geo, Material * mat );
		virtual ~Mesh();

		Model *		geometry;
		Material *	material;
	};
}