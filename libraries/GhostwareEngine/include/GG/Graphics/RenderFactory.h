// Ghostware Games inc. 2016  -Julian Williams
#pragma once

#include <vector>

#include <IwGraphics.h>

#include <GG/Core/Vector.h>
#include "RenderCommand3D.h"
#include "Camera.h"
#include "Material.h"
#include "Model.h"


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
		void		_clearBuffer( const Vector4 & clearColor, uint clearMode ) const;
	

	private:
		void		_render3DList()		const;

	private:
		RenderCommand3DList				_renderCommand3DList;

	};
}
