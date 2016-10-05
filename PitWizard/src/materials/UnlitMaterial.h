#pragma once

#include <GG/Graphics/Material.h>
#include <GG/Core/Matrix.h>


class UnlitMaterial : public GG::BaseMaterial
{
public:
	UnlitMaterial()
	{
	}
	
	virtual void bindMaterialToShader(GG::Shader * shader)
	{
		GG::RenderState * rs = GG::RenderState::getInstance();

		const GG::Matrix4 mvp =	rs->getProjectionMatrix() * 
			rs->getViewMatrix() * 
			rs->getModelMatrix();

		shader->setParameter("inMVP", mvp);
	}
};
