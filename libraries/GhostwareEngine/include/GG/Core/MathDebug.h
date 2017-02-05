// Ghostware Games inc. 2016  -Julian Williams
#pragma once

#include <string>
#include "Vector.h"
#include "Quaternion.h"
#include "Matrix.h"

#include "StringHelper.h"

namespace GG
{
	const std::string ToString( const Vector2 & vec2 );
	const std::string ToString( const Vector3 & vec3 );
	const std::string ToString( const Vector4 & vec4 );
	const std::string ToString(const Vector2i & vec2);
	const std::string ToString(const Vector3i & vec3);
	const std::string ToString(const Vector4i & vec4);
	const std::string ToString( const Matrix4 & mat4 );
}