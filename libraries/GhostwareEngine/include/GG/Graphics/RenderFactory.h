// Ghostware Games inc. 2016  -Julian Williams
#pragma once

#include <vector>

#include <IwGraphics.h>

#include <GG/Core/Vector.h>
#include "RenderCommand3D.h"
#include "Camera.h"
#include "Material.h"
#include "Model.h"
#include "RenderState.h"
#include "Shader.h"

namespace GG
{
	typedef std::vector<RenderCommand3D> RenderCommand3DList;


	class RenderFactory
	{
	public:
		void		addCommand( Material * mat, Model * geo, const Matrix4 & worldMatrix );
		
		void		clearAllCommands();
		void		renderAll( const Camera * camera ) const;


	private:
		void		_opaqueSort( const Vector3 & cameraEye );
		void		_transparentSort( const Vector3 & cameraEye );
			
		void		_setViewport( const Vector4 & viewport ) const;
		void		_clearRenderBuffer( const Vector4 & clearColor, uint clearMode ) const;
		
		void		_setMaterial(const BaseMaterial * material, const Matrix4 & worldMatrix, Camera * camera) const;
		void		_render3DList()		const;

	private:
		Shader *						_tempShader;
		RenderCommand3DList				_renderCommand3DList;
		RenderState						_renderState;
	};
}
