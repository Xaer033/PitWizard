#pragma once

#include "Vector.h"
#include "Matrix.h"
#include "Plane.h"
#include "Sphere.h"
#include "AxisAlignedBox.h"

namespace GG
{

	class Frustum
	{
	public:
		enum PlaneType
		{
			NEAR = 0,
			FAR,
			LEFT,
			RIGHT,
			TOP,
			BOTTOM
		};
		 
		enum class Result
		{
			NONE = 0,
			INSIDE,
			OUTSIDE,
			BOTH
		};

		Frustum();
		Frustum( const Matrix4 & view, const Matrix4 & projection );
		~Frustum();

		void			recreate( const Matrix4 & view, const Matrix4 & projection );

		const Plane&	getPlane( PlaneType type ) const;
		
		Result			intersectBox( const AxisAlignedBox & box ) const;
		Result			intersectSphere( const Sphere & sphere ) const;

	private:
		void			_calculateFrustumPlanes( const Matrix4 & view, const Matrix4 & projection );

	private:
		Plane	_frustumPlanes[ 6 ];
	
	};
}