
#include "Angle.h"

#include <cmath>
#include <glm/glm.hpp>

namespace GG
{
	float cos(const Angle& angle)
	{
		return cosf(angle.toRadians());
	}

	float sin(const Angle& angle)
	{
		return sinf(angle.toRadians());
	}

	float tan(const Angle& angle)
	{
		return tanf(angle.toRadians());
	}

	float acos(const Angle& angle)
	{
		return acosf(angle.toRadians());
	}

	float asin(const Angle& angle)
	{
		return asinf(angle.toRadians());
	}

	float atan(const Angle& angle)
	{
		return atanf(angle.toRadians());
	}
}