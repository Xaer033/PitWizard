
#include "Camera.h"

#include <IwGL.h>
#include <GG/EntitySystem/Entity.h>
#include <GG/Core/Matrix.h>
#include <GG/Core/Log.h>
#include <GG/Core/Angle.h>

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
		setPerspective( Angle::FromDegrees(60.0f), 1.333f, 0.1f, 100 );
		setViewport( 0, 0, 1, 1 );
		setClearColor( Vector4( 0, 0, 0, 1 ) );
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

	void Camera::setPerspective( const Angle & fovy, float aspect, float zNear, float zFar )
	{
		_projectionMat = glm::perspective(fovy.toRadians(), aspect, zNear, zFar);
	}

	void Camera::setOrthogonal( float left, float right, float bottom, float top, float zNear, float zFar )
	{	
		_projectionMat = glm::ortho(left, right, bottom, top, zNear, zFar);
	}

	

	Matrix4 Camera::getViewMatrix( ) const
	{
		const SceneNode * n = getEntity()->getSceneNode();
		if( n != nullptr )
		{

			return glm::inverse(n->modelToWorldMatrix());
		}
		else
		{
			TRACE_WARNING( "Camera is not attached to a scene node!" );
		}

		return _viewMat;
	}

	Matrix4 Camera::getProjectionMatrix( ) const
	{
		return _projectionMat;
	}


}