#include "Application.h"

#include "IwGx.h"
#include "IwUtil.h"
#include "IwResManager.h"
#include "IwGraphics.h"
#include "IwAnim.h"


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
	}

	void Application::shutdown()
	{
		IwResManagerTerminate();
		IwAnimTerminate();
		IwGraphicsTerminate();
		IwGxTerminate();
	}
}