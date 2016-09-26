#pragma once

#include "Vector.h"


namespace GG
{
	class Sphere;

	class AxisAlignedBox
	{
	public:
		nVector3 min;
		nVector3 max;
	
	public:
		AxisAlignedBox();
		AxisAlignedBox( const nVector3 & minimum, const nVector3 & maximum );

		nVector3	getCenter() const;

		bool		intersectsPoint( const nVector3 & point ) const;
		bool		intersectsBox( const AxisAlignedBox & box ) const;
		bool		intersectsSphere( const Sphere & sphere ) const;
	};
}