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
		nVector3 normal;
		float d;

	public:

		Plane();
		Plane( const nVector3 & p, float _d );
		Plane( float a, float b, float c, float _d );
		Plane( const nVector3 & p, const nVector3 & n );
		Plane( const nVector3 & p1, const nVector3 & p2, const nVector3 & p3 );
		~Plane();

		float	getDistance( const nVector3 & point ) const;

		Side	checkSidePoint( const nVector3 & point ) const;
		Side	checkSideBox( const nVector3 & center, const nVector3 & halfSize ) const;
		Side	checkSideBox( const AxisAlignedBox & box ) const;
		Side	checkSideSphere( const nVector3 & center, float radius ) const;
		Side	checkSideSphere( const Sphere & sphere ) const;

		nVector3	projectVector( const nVector3 & point ) const;

	};
}