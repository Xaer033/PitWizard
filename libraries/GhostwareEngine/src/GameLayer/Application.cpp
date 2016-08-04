#include "Application.h"

#include <string>
#include "IwGx.h"
#include "IwResManager.h"
#include "IwGraphics.h"
#include "IwAnim.h"
#include <GG/Core/StringHelper.h>
#include <GG/Core/Log.h>


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
		GG::Log::Init( );

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

		GG::Log::Shutdown();

	}
}