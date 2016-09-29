#include "Application.h"

#include "IwGx.h"
#include "IwUtil.h"
#include "IwResManager.h"
#include "IwGraphics.h"
#include "IwAnim.h"
#include <GG/Core/log.h>

#include "s3e.h"
#include "IwGL.h"


namespace GG
{
	Application::Application()
	{
	}

	Application::~Application()
	{
	}

	void Application::init()
	{
		IwGxInit();
		IwGraphicsInit();
		IwAnimInit();
		IwResManagerInit();
		//s3eSurfaceSetup(S3E_SURFACE_PIXEL_TYPE_RGB888);

		////Initialise graphics system(s)
		//if(!IwGLInit())
		//{
		//	LOG_ERROR("IwGLInit failed");
		//	return;
		//}

		//IwGLSwapBuffers();
		//s3eDeviceYield(0);
	}

	void Application::shutdown()
	{
		IwResManagerTerminate();
		IwAnimTerminate();
		IwGraphicsTerminate();
		IwGxTerminate();
		//IwGLTerminate();
	}
}