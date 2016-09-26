#pragma once

#include "Vector.h"

namespace GG
{
	class AxisAlignedBox;

	class Sphere
	{
	public:
		Vector3		center;
		float		radius;

	public:
		Sphere();
		Sphere( const Vector3 & pCenter, float pRadius );
		~Sphere();

		void	set( const Vector3 & pCenter, float pRadius );

		bool	intersectsPoint( const Vector3 & point ) const;
		bool	intersectsBox( const AxisAlignedBox & box ) const;
		bool	intersectsSphere( const Sphere & sphere ) const;
	};
}