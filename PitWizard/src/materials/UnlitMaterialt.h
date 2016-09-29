#pragma once

#include <GG/Graphics/Material.h>



class UnlitMaterial : public GG::BaseMaterial
{
public:
	UnlitMaterial()
	{
	}
	
	virtual void bindMaterialToShader(
		const GG::RenderState & renderState,
		const GG::Matrix4 & worldMatrix,
		const GG::Camera *	camera,
		const GG::Shader *	shader) const
	{
		
	}
};
