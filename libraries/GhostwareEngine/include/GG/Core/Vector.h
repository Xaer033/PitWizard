// Ghostware Games inc. 2016  -Julian Williams
#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>

namespace GG
{
	typedef glm::vec2	Vector2;
	typedef glm::vec3	Vector3;
	typedef glm::vec4	Vector4;

	typedef glm::ivec2	Vector2i;
	typedef glm::ivec3	Vector3i;
	typedef glm::ivec4	Vector4i;

	namespace Vector
	{
		inline Vector3 up()
		{
			static Vector3 up = Vector3(0, 1, 0);
			return up;
		}

		inline Vector3 down()
		{
			static Vector3 down = Vector3(0, -1, 0);
			return down;
		}

		inline Vector3 left()
		{
			static Vector3 left = Vector3(-1, 0, 0);
			return left;
		}

		inline Vector3 right()
		{
			static Vector3 right = Vector3(1, 0, 0);
			return right;
		}

		inline Vector3 forward()
		{
			static Vector3 forward = Vector3(0, 0, 1);
			return forward;
		}

		inline Vector3 back()
		{
			static Vector3 back = Vector3(0, 0, -1);
			return back;
		}

		inline Vector3 zero()
		{
			static Vector3 zero = Vector3(0, 0, 0);
			return zero;
		}
	}
}
