
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

	const std::string ToString( const nMatrix4 & mat4 )
	{
		std::string matrixFmt;

		StringHelper::Format(matrixFmt, "{ %.4f, %.4f, %.4f, %.4f }\n{ %.4f, %.4f, %.4f, %.4f }\n{ %.4f, %.4f, %.4f, %.4f }\n{ %.4f, %.4f, %.4f, %.4f }",
			mat4[0][0], mat4[1][0], mat4[2][0], mat4[3][0],
			mat4[0][1], mat4[1][1], mat4[2][1], mat4[3][1],
			mat4[0][2], mat4[1][2], mat4[2][2], mat4[3][2],
			mat4[0][3], mat4[1][3], mat4[2][3], mat4[3][3]);

		return matrixFmt;
	}
}