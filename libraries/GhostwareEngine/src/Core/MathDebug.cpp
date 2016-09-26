
#include "MathDebug.h"

namespace GG
{
	const std::string ToString( const nVector2 & vec2 )
	{
		std::string str;
		StringHelper::Format( str, "{ %f, %f }", vec2.x, vec2.y );
		return str;
	}

	const std::string ToString( const nVector3 & vec3 )
	{
		std::string str;
		StringHelper::Format( str, "{ %f, %f, %f }", vec3.x, vec3.y, vec3.z );
		return str;
	}

	const std::string ToString( const nVector4 & vec4 )
	{
		std::string str;
		StringHelper::Format( str, "{ %f, %f, %f, %f }", vec4.x, vec4.y, vec4.z, vec4.w );
		return str;
	}

	const std::string ToString( const Matrix4 & mat4 )
	{
		std::string matrixFmt;

		StringHelper::Format( matrixFmt, "R={ %.4f, %.4f, %.4f }\n  { %.4f, %.4f, %.4f }\n  { %.4f, %.4f, %.4f }\nT={ %.4f, %.4f, %.4f }",
			mat4.m[ 0 ][ 0 ], mat4.m[ 1 ][ 0 ], mat4.m[ 2 ][ 0 ],
			mat4.m[ 0 ][ 1 ], mat4.m[ 1 ][ 1 ], mat4.m[ 2 ][ 1 ],
			mat4.m[ 0 ][ 2 ], mat4.m[ 1 ][ 2 ], mat4.m[ 2 ][ 2 ],
			mat4.t.x, mat4.t.y, mat4.t.z );

		return matrixFmt;
	}
}