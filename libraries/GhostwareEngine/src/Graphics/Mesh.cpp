
#include "Mesh.h"

#include "Material.h"
#include "Model.h"
#include <GG/Core/Log.h>


namespace GG
{
	Mesh::Mesh( Model * geo, Material * mat ) :
		geometry( geo ), material( mat )
	{
	}
	
	Mesh::~Mesh()
	{
		geometry = nullptr;
		material = nullptr;
	}
}
