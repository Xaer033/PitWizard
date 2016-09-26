#pragma once

#include "Vector.h"

namespace GG
{
	class AxisAlignedBox;
	class Sphere;


	class Plane
	{
	public:

		enum class Side
		{
			NONE = 0,
			POSITIVE,
			NEGITIVE,
			BOTH
		};

	public:
		Vector3 normal;
		float d;

	public:

		Plane();
		Plane( const Vector3 & p, float _d );
		Plane( float a, float b, float c, float _d );
		Plane( const Vector3 & p, const Vector3 & n );
		Plane( const Vector3 & p1, const Vector3 & p2, const Vector3 & p3 );
		~Plane();

		float	getDistance( const Vector3 & point ) const;

		Side	checkSidePoint( const Vector3 & point ) const;
		Side	checkSideBox( const Vector3 & center, const Vector3 & halfSize ) const;
		Side	checkSideBox( const AxisAlignedBox & box ) const;
		Side	checkSideSphere( const Vector3 & center, float radius ) const;
		Side	checkSideSphere( const Sphere & sphere ) const;

		Vector3	projectVector( const Vector3 & point ) const;

	};
}