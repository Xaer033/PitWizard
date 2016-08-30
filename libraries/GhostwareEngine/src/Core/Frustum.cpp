
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

		_frustumPlanes[ PlaneType::LEFT ].normal.x = combo.m[ 3 ][ 0 ] + combo.m[ 0 ][ 0 ];
		_frustumPlanes[ PlaneType::LEFT ].normal.y = combo.m[ 3 ][ 1 ] + combo.m[ 0 ][ 1 ];
		_frustumPlanes[ PlaneType::LEFT ].normal.z = combo.m[ 3 ][ 2 ] + combo.m[ 0 ][ 2 ];
		_frustumPlanes[ PlaneType::LEFT ].d = combo.m[ 3 ][ 3 ] + combo.m[ 0 ][ 3 ];

		_frustumPlanes[ PlaneType::RIGHT ].normal.x = combo.m[ 3 ][ 0 ] - combo.m[ 0 ][ 0 ];
		_frustumPlanes[ PlaneType::RIGHT ].normal.y = combo.m[ 3 ][ 1 ] - combo.m[ 0 ][ 1 ];
		_frustumPlanes[ PlaneType::RIGHT ].normal.z = combo.m[ 3 ][ 2 ] - combo.m[ 0 ][ 2 ];
		_frustumPlanes[ PlaneType::RIGHT ].d = combo.m[ 3 ][ 3 ] - combo.m[ 0 ][ 3 ];

		_frustumPlanes[ PlaneType::TOP ].normal.x = combo.m[ 3 ][ 0 ] - combo.m[ 1 ][ 0 ];
		_frustumPlanes[ PlaneType::TOP ].normal.y = combo.m[ 3 ][ 1 ] - combo.m[ 1 ][ 1 ];
		_frustumPlanes[ PlaneType::TOP ].normal.z = combo.m[ 3 ][ 2 ] - combo.m[ 1 ][ 2 ];
		_frustumPlanes[ PlaneType::TOP ].d = combo.m[ 3 ][ 3 ] - combo.m[ 1 ][ 3 ];

		_frustumPlanes[ PlaneType::BOTTOM ].normal.x = combo.m[ 3 ][ 0 ] + combo.m[ 1 ][ 0 ];
		_frustumPlanes[ PlaneType::BOTTOM ].normal.y = combo.m[ 3 ][ 1 ] + combo.m[ 1 ][ 1 ];
		_frustumPlanes[ PlaneType::BOTTOM ].normal.z = combo.m[ 3 ][ 2 ] + combo.m[ 1 ][ 2 ];
		_frustumPlanes[ PlaneType::BOTTOM ].d = combo.m[ 3 ][ 3 ] + combo.m[ 1 ][ 3 ];

		_frustumPlanes[ PlaneType::NEAR ].normal.x = combo.m[ 3 ][ 0 ] + combo.m[ 2 ][ 0 ];
		_frustumPlanes[ PlaneType::NEAR ].normal.y = combo.m[ 3 ][ 1 ] + combo.m[ 2 ][ 1 ];
		_frustumPlanes[ PlaneType::NEAR ].normal.z = combo.m[ 3 ][ 2 ] + combo.m[ 2 ][ 2 ];
		_frustumPlanes[ PlaneType::NEAR ].d = combo.m[ 3 ][ 3 ] + combo.m[ 2 ][ 3 ];

		_frustumPlanes[ PlaneType::FAR ].normal.x = combo.m[ 3 ][ 0 ] - combo.m[ 2 ][ 0 ];
		_frustumPlanes[ PlaneType::FAR ].normal.y = combo.m[ 3 ][ 1 ] - combo.m[ 2 ][ 1 ];
		_frustumPlanes[ PlaneType::FAR ].normal.z = combo.m[ 3 ][ 2 ] - combo.m[ 2 ][ 2 ];
		_frustumPlanes[ PlaneType::FAR ].d = combo.m[ 3 ][ 3 ] - combo.m[ 2 ][ 3 ];

		// Renormalise any normals which were not unit length
		for( int i = 0; i < 6; ++i )
		{
			_frustumPlanes[ i ].normal.Normalise();
			_frustumPlanes[ i ].d /= _frustumPlanes[i].normal.GetLength();
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