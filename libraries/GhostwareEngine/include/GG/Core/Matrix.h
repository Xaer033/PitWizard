#pragma once

#include <glm/glm.hpp>
#include <glm/matrix.hpp>
#include "glm/gtx/transform.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <IwGeom.h>

namespace GG
{
	typedef CIwFMat		Matrix4;
	typedef glm::mat4	nMatrix4;
	namespace Matrix
	{

		inline nMatrix4 identity()
		{
			static nMatrix4 identity = nMatrix4(1);
			return identity;
		}

		inline Matrix4 matConvert(const nMatrix4 & m)
		{
			Matrix4 outMat;
			outMat.t.x = m[3][0];
			outMat.t.y = m[3][1];
			outMat.t.z = m[3][2];

			nMatrix4 transpose = glm::transpose(m);
			for(int y = 0; y < 3; ++y)
			{
				for(int x = 0; x < 3; ++x)
				{
					outMat.m[x][y] = transpose[x][y];
				}
			}
			return outMat;
		}

		/*inline nMatrix4 lookAt(
			const nVector3 & eye,
			const nVector3 & center,
			const nVector3 & up )
		{
			nMatrix4 lookMat	= glm::lookAt(eye, center, up);
			lookMat[3][0]		= -lookMat[3][0];
			lookMat[3][1]		= -lookMat[3][1];
			lookMat[3][2]		= -lookMat[3][2];
			return lookMat;
		}*/
	}
}
