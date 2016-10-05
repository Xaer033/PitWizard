#pragma once

#include <glm/glm.hpp>
#include <glm/matrix.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace GG
{
	typedef glm::mat4	Matrix4;
	namespace Matrix
	{

		inline Matrix4 identity()
		{
			static Matrix4 identity = Matrix4(1);
			return identity;
		}
/*
		inline CIwFMat matConvert(const Matrix4 & m)
		{
			CIwFMat outMat;
		

			Matrix4 transpose = glm::transpose(m);
			for(int y = 0; y < 3; ++y)
			{
				for(int x = 0; x < 3; ++x)
				{
					outMat.m[x][y] = transpose[x][y];
				}
			}
			outMat.t.x = m[3][0];
			outMat.t.y = m[3][1];
			outMat.t.z = m[3][2];
			return outMat;
		}
*/
		/*inline Matrix4 lookAt(
			const nVector3 & eye,
			const nVector3 & center,
			const nVector3 & up )
		{
			Matrix4 lookMat	= glm::lookAt(eye, center, up);
			lookMat[3][0]		= -lookMat[3][0];
			lookMat[3][1]		= -lookMat[3][1];
			lookMat[3][2]		= -lookMat[3][2];
			return lookMat;
		}*/
	}
}
