// Ghostware Games inc. 2016  -Julian Williams
#pragma once

#include <vector>

#include <GG/Core/Vector.h>
#include "RenderCommand3D.h"
#include "Camera.h"
#include "Material.h"
#include "Model.h"
#include "RenderState.h"
#include "Shader.h"
#include "GG/Resources//ResourceHandle.h"

namespace GG
{
	typedef std::vector<RenderCommand3D> RenderCommand3DList;


	class RenderFactory
	{
	public:
		RenderFactory();
		~RenderFactory();

		void		addCommand(Material * mat, Model * geo, const Matrix4 & worldMatrix );
		
		void		clearAllCommands();
		void		renderAll( const Camera * camera );

		void		loadTempShader();

	private:
		void		_opaqueSort( const Vector3 & cameraEye );
		void		_transparentSort( const Vector3 & cameraEye );
			
		void		_setViewport( const Vector4 & viewport ) const;
		void		_clearRenderBuffer( const Vector4 & clearColor, uint clearMode ) const;
		
		void		_setMaterial( Material * material);
		void		_render3DList(const Camera * camera);

	
	private:
		Shader *						_tempShader;
		RenderCommand3DList				_renderCommand3DList;

		ResourceHandle<Texture2D>		_tempTexture;
		ResourceHandle<Texture2D>		_roughnessTex;
		ResourceHandle<Texture2D>		_normalTex;
	};
}
