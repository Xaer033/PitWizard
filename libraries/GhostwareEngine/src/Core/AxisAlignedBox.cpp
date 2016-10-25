
#include "AxisAlignedBox.h"

#include "Vector.h"
#include "Sphere.h"

namespace GG
{
	AxisAlignedBox::AxisAlignedBox() :
		min( Vector::zero() ),
		max( Vector::zero() )
	{
	}

	AxisAlignedBox::AxisAlignedBox( 
		const Vector3 & minimum, 
		const Vector3 & maximum ) :
		min( minimum ),
		max( maximum )
	{
	}

	Vector3 AxisAlignedBox::getCenter() const
	{
		return (min + max) / 2.0f;
	}

	bool	AxisAlignedBox::intersectsPoint( const Vector3 & point ) const
	{
		return	point.x >= min.x && point.x <= max.x &&
				point.y >= min.y && point.y <= max.y &&
				point.z >= min.z && point.z <= max.z;
	}

	bool	AxisAlignedBox::intersectsBox( const AxisAlignedBox & box ) const
	{
		if( max.x < box.min.x )
			return false;
		if( max.y < box.min.y )
			return false;
		if( max.z < box.min.z )
			return false;

		if( min.x > box.max.x )
			return false;
		if( min.y > box.max.y )
			return false;
		if( min.z > box.max.z )
			return false;
		
		return true;
	}

	bool	AxisAlignedBox::intersectsSphere( const Sphere & sphere ) const
	{
		return false;
	}

}