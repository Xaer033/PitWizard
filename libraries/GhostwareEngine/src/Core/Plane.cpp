
#include "Plane.h"

#include <cmath>

#include "Sphere.h"
#include "AxisAlignedBox.h"
#include "Matrix.h"
#include "Vector.h"


namespace GG
{
	Plane::Plane() : 
		normal(0, 0, 0), 
		d(0)
	{
	}

	Plane::Plane( const nVector3 & n, float _d ) : 
		normal( n ), 
		d( -_d )
	{
	}

	Plane::Plane( 
		const nVector3 & p, 
		const nVector3 & n ) : 
		normal( n )
	{
		d = -glm::dot(p, n);
	}

	Plane::Plane( float a, float b, float c, float _d ) :
		normal(a, b, c),
		d(_d)
	{
	}

	Plane::Plane( const nVector3 & p1, const nVector3 & p2, const nVector3 & p3 )
	{
		const nVector3 delta1 = p2 - p1;
		const nVector3 delta2 = p3 - p1;
		normal = glm::cross(delta1, delta2);
		normal = glm::normalize(normal);
		d = -glm::dot(normal, p1);
	}

	Plane::~Plane()
	{
	}
	
	
	float Plane::getDistance( const nVector3 & point ) const
	{
		return glm::dot(normal, point) + d;
	}
	
	Plane::Side Plane::checkSidePoint( const nVector3 & point ) const
	{
		float distance = getDistance( point );
		if( distance > 0 )
			return Plane::Side::POSITIVE;

		if( distance < 0 )
			return Plane::Side::NEGITIVE;

		return Plane::Side::BOTH;
	}

	Plane::Side Plane::checkSideBox( const nVector3 & center, const nVector3 & halfsize ) const
	{
		float distance	= getDistance( center );
		float maxDist	= fabsf( glm::dot(normal, halfsize ) );

		if( distance > maxDist )
			return Plane::Side::POSITIVE;

		if( distance < -maxDist )
			return Plane::Side::NEGITIVE;

		return Plane::Side::BOTH;
	}

	Plane::Side Plane::checkSideBox( const AxisAlignedBox & box ) const
	{
		nVector3 boxCenter	= box.getCenter();
		nVector3 halfsize	= box.max - boxCenter;
		return checkSideBox( boxCenter, halfsize );
	}

	Plane::Side Plane::checkSideSphere( const nVector3 & center, float radius ) const
	{
		float distance = getDistance( center );
		if( distance > radius )
			return Plane::Side::POSITIVE;

		if( distance < -radius )
			return Plane::Side::NEGITIVE;

		return Plane::Side::BOTH;
	}

	Plane::Side Plane::checkSideSphere( const Sphere & sphere ) const
	{
		return checkSideSphere( sphere.center, sphere.radius );
	}

	nVector3 Plane::projectVector( const nVector3 & point ) const
	{
		// Referenced Ogre3d
		nMatrix4 xform;
		xform[ 0 ][ 0 ] = 1.0f - normal.x * normal.x;
		xform[ 0 ][ 1 ] = -normal.x * normal.y;
		xform[ 0 ][ 2 ] = -normal.x * normal.z;
		xform[ 1 ][ 0 ] = -normal.y * normal.x;
		xform[ 1 ][ 1 ] = 1.0f - normal.y * normal.y;
		xform[ 1 ][ 2 ] = -normal.y * normal.z;
		xform[ 2 ][ 0 ] = -normal.z * normal.x;
		xform[ 2 ][ 1 ] = -normal.z * normal.y;
		xform[ 2 ][ 2 ] = 1.0f - normal.z * normal.z;
		nVector4 newPoint = xform * nVector4(point, 1);
		return nVector3(newPoint.x, newPoint.y, newPoint.z);
	}
}