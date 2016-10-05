
#include "Mesh.h"

#include "Material.h"
#include "Model.h"
#include <GG/Core/Log.h>


namespace GG
{
	Mesh::Mesh() :
		geometry(nullptr),
		material(nullptr)
	{
	}

	Mesh::Mesh( Model * geo, BaseMaterial * mat ) :
		geometry( geo ), material( mat )
	{
	}
	
	Mesh::~Mesh()
	{
		geometry = nullptr;
		material = nullptr;
	}

	void Mesh::render( RenderFactory & renderFactory )
	{
		renderFactory.addCommand(
			material,
			geometry,
			_entity->getSceneNode()->modelToWorldMatrix()
		);
	}
}
