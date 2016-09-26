
#include "Sphere.h"

#include "Vector.h"
#include "AxisAlignedBox.h"

namespace GG
{
	Sphere::Sphere() : 
		center( Vector::zero() ), 
		radius( 0 )
	{
	}

	Sphere::Sphere( const nVector3 & pCenter, float pRadius ) :
		center( pCenter ),
		radius( pRadius )
	{
	}

	void Sphere::set( const nVector3 & pCenter, float pRadius )
	{
		center = pCenter;
		radius = pRadius;
	}

	bool Sphere::intersectsPoint( const nVector3 & point ) const
	{
		return glm::length2( center - point ) < (radius * radius);
	}

	bool Sphere::intersectsBox( const AxisAlignedBox & box ) const
	{
		return box.intersectsSphere( *this );
	}
}