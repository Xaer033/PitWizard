#include "Application.h"


#include "IwGx.h"
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
		IwResManagerInit();
		IwGraphicsInit();
		IwAnimInit();
	}

	void Application::shutdown()
	{
		IwAnimTerminate();
		IwGraphicsTerminate();
		IwResManagerTerminate();
		IwGxTerminate();
	}
}