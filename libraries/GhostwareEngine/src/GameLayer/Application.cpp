#include "Application.h"

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
		/*IwGxInit();
		IwGraphicsInit();
		IwAnimInit();
		IwResManagerInit();*/
		
	}

	void Application::shutdown()
	{
		/*IwResManagerTerminate();
		IwAnimTerminate();
		IwGraphicsTerminate();
		IwGxTerminate();*/
	}
}