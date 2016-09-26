// Ghostware Games inc. 2016  -Julian Williams
#pragma once

#include <string>
#include "Vector.h"
#include "Quaternion.h"
#include "Matrix.h"

#include "StringHelper.h"

namespace GG
{
	const std::string ToString( const nVector2 & vec2 );
	const std::string ToString( const nVector3 & vec3 );
	const std::string ToString( const nVector4 & vec4 );
	const std::string ToString( const Matrix4 & mat4 );
}