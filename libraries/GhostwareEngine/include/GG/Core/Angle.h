// Ghostware Games inc. 2016  -Julian Williams
#pragma once

#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

namespace GG
{
#define TO_RADIANS(x) x * glm::pi<float>() / 180.0f
#define TO_DEGREES(x) x * 180.0f / glm::pi<float>()

	class Angle
	{
	public:
		Angle() : value(0)
		{
		}

		static Angle FromDegrees(float degree)
		{
			return Angle(TO_RADIANS(degree));
		}

		static Angle FromRadians(float radii)
		{
			return Angle(radii);
		}


		inline float toDegrees() const
		{
			return TO_DEGREES(value);
		}

		inline float toRadians() const
		{
			return value;
		}

		friend Angle operator- (const Angle & x, const Angle & y)
		{
			return Angle(x.value - y.value);
		}
	
		friend Angle operator+ (const Angle & x, const Angle & y)
		{
			return Angle(x.value + y.value);
		}
		friend Angle operator* (const Angle & x, float y)
		{
			return Angle(x.value * y);
		}
	private:
		Angle(float pValue) : value(pValue)
		{
		}

		float value;
	};


	float cos(const Angle & angle);
	float sin(const Angle & angle);
	float tan(const Angle & angle);
	float acos(const Angle & angle);
	float asin(const Angle & angle);
	float atan(const Angle & angle);
}