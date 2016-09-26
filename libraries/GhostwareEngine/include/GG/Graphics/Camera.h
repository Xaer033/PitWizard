// Ghostware Games inc. 2016  -Julian Williams
#pragma once

#include "RenderableObject.h"
#include "IComponent.h"

#include <GG/Core/Matrix.h>
#include <GG/Core/Vector.h>
#include <GG/Core/Types.h>

#include <string>

#include "IwHashString.h"
#include "IwGx.h"

namespace GG
{
	enum ClearMode
	{
		None	= 0,
		Color	= IW_GX_COLOUR_BUFFER_F,
		Depth	= IW_GX_DEPTH_BUFFER_F
	};


	class Camera : public IComponent
	{
	public:
		static	bool DepthCompare( const Camera * a, const Camera * b );

	public:
		Camera();
		virtual ~Camera();

		static const uint32 GetComponentId()
		{
			static uint32 id = IwHashString( "CameraComponent" );
			return id;
		}

		void				setEnabled( bool enabled );
		bool				isEnabled() const;
		
		void				setRenderLayer( int32 layer );
		int32				getRenderLayer() const;
		
		void				setDepth( int16 depth );
		int16				getDepth( ) const;
		
		void				setClearMode( uint clearMode);
		uint				getClearMode( ) const;
		
		void				setClearColor( const nVector4 & clearColor );
		nVector4				getClearColor( ) const;
		
		void				setViewport( float x, float y, float width, float height );
		nVector4				getViewport() const;


		void				setOrthogonal( float left, float right, float bottom, float top, float zNear, float zFar );
		void				setPerspective( float fovy, float aspect, float zNear, float zFar );


		Matrix4				getViewMatrix() const;
		const float *		getProjectionMatrix() const ;

	private:

		bool				_enabled;
		
		int16				_depth;

		int32				_layer;

		uint				_clearMode;

		std::string			_name;

		Matrix4				_viewMat;
		float 				_projectionMat[16];

		nVector4				_cachedClearColor;
		nVector4				_cachedViewport;
	};
}
