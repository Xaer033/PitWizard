// Ghostware Games inc. 2016  -Julian Williams
#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>
#include <IwGeom.h>

namespace GG
{
	typedef CIwFVec2 Vector2;
	typedef CIwFVec3 Vector3;
	typedef CIwFVec4 Vector4;

	typedef glm::vec2 nVector2;
	typedef glm::vec3 nVector3;
	typedef glm::vec4 nVector4;

	namespace Vector
	{
		inline nVector3 up()
		{
			static nVector3 up = nVector3(0, 1, 0);
			return up;
		}

		inline nVector3 down()
		{
			static nVector3 down = nVector3(0, -1, 0);
			return down;
		}

		inline nVector3 left()
		{
			static nVector3 left = nVector3(-1, 0, 0);
			return left;
		}

		inline nVector3 right()
		{
			static nVector3 right = nVector3(1, 0, 0);
			return right;
		}

		inline nVector3 forward()
		{
			static nVector3 forward = nVector3(0, 0, 1);
			return forward;
		}

		inline nVector3 back()
		{
			static nVector3 back = nVector3(0, 0, -1);
			return back;
		}

		inline nVector3 zero()
		{
			static nVector3 zero = nVector3(0, 0, 0);
			return zero;
		}
	}
}
