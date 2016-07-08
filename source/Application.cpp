#include <GG/Application.h>


#include "IwGx.h"


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
	}

	void Application::shutdown()
	{
		IwGxTerminate();
	}
}