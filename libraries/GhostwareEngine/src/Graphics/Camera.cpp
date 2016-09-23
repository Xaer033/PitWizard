
#include "Camera.h"

#include <IwGx.h>
#include <GG/EntitySystem/Entity.h>
#include <GG/Core/Matrix.h>
#include <GG/Core/Log.h>

#include "RenderableObject.h"



namespace GG
{
	bool Camera::DepthCompare( const Camera * a, const Camera * b )
	{
		if( a == nullptr || b == nullptr )
			return false;

		return a->getDepth() < b->getDepth();
	}


	Camera::Camera() : 
		_enabled(true), 
		_depth(0), 
		_layer(0)
	{
		setPerspective( 60, 1.333f, 0.1f, 100 );
		setViewport( 0, 0, 1, 1 );
		setClearColor( Vector4( 0, 0, 0, 0 ) );
	}
	
	Camera::~Camera()
	{
	}

	inline void Camera::setEnabled( bool enabled )
	{
		_enabled = enabled;
	}

	inline bool Camera::isEnabled( ) const
	{
		return _enabled;
	}

	inline void Camera::setRenderLayer( int32 layer )
	{
		_layer = layer;
	}

	inline int32 Camera::getRenderLayer( ) const
	{
		return _layer;
	}

	inline void Camera::setDepth( int16 depth )
	{
		_depth = depth;
	}

	inline int16 Camera::getDepth() const
	{
		return _depth;
	}

	void Camera::setClearColor( const Vector4 & clearColor )
	{
		if( clearColor == _cachedClearColor )
			return;

		_cachedClearColor = clearColor;
	}

	Vector4 Camera::getClearColor( ) const
	{
		return _cachedClearColor;
	}

	void Camera::setClearMode( uint clearMode )
	{
		_clearMode = clearMode;
	}
	
	uint Camera::getClearMode( ) const
	{
		return _clearMode;
	}

	void Camera::setViewport( float x, float y, float width, float height )
	{
		_cachedViewport = Vector4( x, y, width, height );
	}

	Vector4 Camera::getViewport() const
	{
		return _cachedViewport;
	}

	void Camera::setPerspective( float fovy, float aspect, float zNear, float zFar )
	{
		const float D2R = PI / 180.0f;

		float	yScale		= 1.0f / (float)tan( D2R * fovy / 2.0f );
		float	xScale		= yScale / aspect;
		float	nearmfar	= zNear - zFar;

		_projectionMat[ 0 ]	= xScale;
		_projectionMat[ 1 ]	= 0;
		_projectionMat[ 2 ] = 0;
		_projectionMat[ 3 ] = 0;


		_projectionMat[ 4 ] = 0;
		_projectionMat[ 5 ] = yScale;
		_projectionMat[ 6 ] = 0;
		_projectionMat[ 7 ] = 0;

		_projectionMat[ 8 ] = 0;
		_projectionMat[ 9 ] = 0;
		_projectionMat[ 10] = ( zFar + zNear ) / nearmfar;
		_projectionMat[ 11] = -1;

		_projectionMat[ 12] = 0;
		_projectionMat[ 13] = 0;
		_projectionMat[ 14] = 2 * zFar*zNear / nearmfar;
		_projectionMat[ 15] = 0;
	}

	void Camera::setOrthogonal( float left, float right, float bottom, float top, float zNear, float zFar )
	{	
		//IwGxSetOrtho()
	}

	

	Matrix4 Camera::getViewMatrix( ) const
	{
		const SceneNode * n = getEntity()->getSceneNode();
		if( n != nullptr )
		{
			return n->modelToWorldMatrix();
		}
		else
		{
			TRACE_WARNING( "Camera is not attached to a scene node!" );
		}

		return _viewMat;
	}

	const float * Camera::getProjectionMatrix( ) const
	{
		return _projectionMat;
	}


}