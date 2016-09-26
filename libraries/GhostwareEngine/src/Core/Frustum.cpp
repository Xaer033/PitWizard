
#include "Frustum.h"

#include <cmath>
#include "AxisAlignedBox.h"

namespace GG
{
	Frustum::Frustum() 
	{
	}

	Frustum::Frustum( const Matrix4 & view, const Matrix4 & projection )
	{
		recreate( view, projection );
	}

	Frustum::~Frustum()
	{
	}

	void Frustum::recreate( const Matrix4 & view, const Matrix4 & projection )
	{
		_calculateFrustumPlanes( view, projection );
	}

	void Frustum::_calculateFrustumPlanes( const Matrix4 & view, const Matrix4 & projection )
	{
		Matrix4 combo = projection * view;

		_frustumPlanes[ PlaneType::LEFT ].normal.x = combo[ 3 ][ 0 ] + combo[ 0 ][ 0 ];
		_frustumPlanes[ PlaneType::LEFT ].normal.y = combo[ 3 ][ 1 ] + combo[ 0 ][ 1 ];
		_frustumPlanes[ PlaneType::LEFT ].normal.z = combo[ 3 ][ 2 ] + combo[ 0 ][ 2 ];
		_frustumPlanes[ PlaneType::LEFT ].d = combo[ 3 ][ 3 ] + combo[ 0 ][ 3 ];

		_frustumPlanes[ PlaneType::RIGHT ].normal.x = combo[ 3 ][ 0 ] - combo[ 0 ][ 0 ];
		_frustumPlanes[ PlaneType::RIGHT ].normal.y = combo[ 3 ][ 1 ] - combo[ 0 ][ 1 ];
		_frustumPlanes[ PlaneType::RIGHT ].normal.z = combo[ 3 ][ 2 ] - combo[ 0 ][ 2 ];
		_frustumPlanes[ PlaneType::RIGHT ].d = combo[ 3 ][ 3 ] - combo[ 0 ][ 3 ];

		_frustumPlanes[ PlaneType::TOP ].normal.x = combo[ 3 ][ 0 ] - combo[ 1 ][ 0 ];
		_frustumPlanes[ PlaneType::TOP ].normal.y = combo[ 3 ][ 1 ] - combo[ 1 ][ 1 ];
		_frustumPlanes[ PlaneType::TOP ].normal.z = combo[ 3 ][ 2 ] - combo[ 1 ][ 2 ];
		_frustumPlanes[ PlaneType::TOP ].d = combo[ 3 ][ 3 ] - combo[ 1 ][ 3 ];

		_frustumPlanes[ PlaneType::BOTTOM ].normal.x = combo[ 3 ][ 0 ] + combo[ 1 ][ 0 ];
		_frustumPlanes[ PlaneType::BOTTOM ].normal.y = combo[ 3 ][ 1 ] + combo[ 1 ][ 1 ];
		_frustumPlanes[ PlaneType::BOTTOM ].normal.z = combo[ 3 ][ 2 ] + combo[ 1 ][ 2 ];
		_frustumPlanes[ PlaneType::BOTTOM ].d = combo[ 3 ][ 3 ] + combo[ 1 ][ 3 ];

		_frustumPlanes[ PlaneType::NEAR ].normal.x = combo[ 3 ][ 0 ] + combo[ 2 ][ 0 ];
		_frustumPlanes[ PlaneType::NEAR ].normal.y = combo[ 3 ][ 1 ] + combo[ 2 ][ 1 ];
		_frustumPlanes[ PlaneType::NEAR ].normal.z = combo[ 3 ][ 2 ] + combo[ 2 ][ 2 ];
		_frustumPlanes[ PlaneType::NEAR ].d = combo[ 3 ][ 3 ] + combo[ 2 ][ 3 ];

		_frustumPlanes[ PlaneType::FAR ].normal.x = combo[ 3 ][ 0 ] - combo[ 2 ][ 0 ];
		_frustumPlanes[ PlaneType::FAR ].normal.y = combo[ 3 ][ 1 ] - combo[ 2 ][ 1 ];
		_frustumPlanes[ PlaneType::FAR ].normal.z = combo[ 3 ][ 2 ] - combo[ 2 ][ 2 ];
		_frustumPlanes[ PlaneType::FAR ].d = combo[ 3 ][ 3 ] - combo[ 2 ][ 3 ];

		// Renormalise any normals which were not unit length
		for( int i = 0; i < 6; ++i )            
		{
			_frustumPlanes[i].normal = glm::normalize(_frustumPlanes[i].normal);
			_frustumPlanes[i].d /= glm::length(_frustumPlanes[i].normal);
		}
	}

	const Plane& Frustum::getPlane( PlaneType type ) const
	{
		return _frustumPlanes[ type ];
	}

	Frustum::Result Frustum::intersectBox( const AxisAlignedBox & box ) const
	{
		for( int i = 0; i < 6; ++i )
		{
			if( _frustumPlanes[ i ].checkSideBox( box ) == Plane::Side::POSITIVE )
				return Frustum::Result::OUTSIDE;
		}
		return Frustum::Result::INSIDE;
	}

	Frustum::Result Frustum::intersectSphere( const Sphere & sphere ) const
	{
		for( int i = 0; i < 6; ++i )
		{
			if( _frustumPlanes[ i ].checkSideSphere( sphere ) == Plane::Side::POSITIVE )
				return Frustum::Result::OUTSIDE;
		}
		return Frustum::Result::INSIDE;
	}


}