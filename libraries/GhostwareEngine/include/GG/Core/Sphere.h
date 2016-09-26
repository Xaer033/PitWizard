#pragma once

#include "Vector.h"

namespace GG
{
	class AxisAlignedBox;

	class Sphere
	{
	public:
		nVector3	center;
		float		radius;

	public:
		Sphere();
		Sphere( const nVector3 & pCenter, float pRadius );
		~Sphere();

		void	set( const nVector3 & pCenter, float pRadius );

		bool	intersectsPoint( const nVector3 & point ) const;
		bool	intersectsBox( const AxisAlignedBox & box ) const;
		bool	intersectsSphere( const Sphere & sphere ) const;
	};
}