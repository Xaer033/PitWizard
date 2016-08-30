#pragma once

#include "Vector.h"


namespace GG
{
	class Sphere;

	class AxisAlignedBox
	{
	public:
		Vector3 min;
		Vector3 max;
	
	public:
		AxisAlignedBox();
		AxisAlignedBox( const Vector3 & minimum, const Vector3 & maximum );

		Vector3		getCenter() const;

		bool		intersectsPoint( const Vector3 & point ) const;
		bool		intersectsBox( const AxisAlignedBox & box ) const;
		bool		intersectsSphere( const Sphere & sphere ) const;
	};
}