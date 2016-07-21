#pragma once

namespace GG
{
	struct Rect
	{
		float x;
		float y;
		float w;
		float h;

		Rect() : x( 0 ), y( 0 ), w( 0 ), h( 0 )
		{
		}

		Rect( float _x, float _y, float _w, float _h ) :
			x( _x ), y( _y ), w( _x + _w ), h( y + _h )
		{
		}

		void set( float _x, float _y, float _w, float _h )
		{
			x = _x;
			y = _y;
			w = _x + _w;
			h = _y + _h;
		}

		static bool Intersect( const Rect & a, const Rect & b )
		{
			return	 !( b.x > a.w ||
						b.w < a.x ||
						b.y > a.h ||
						b.h < a.y );
		}
	};
}
