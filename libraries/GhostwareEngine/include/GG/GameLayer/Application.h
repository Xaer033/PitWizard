#pragma once // Ghostware Games inc. 2016  -Julian Williams


namespace GG
{
	class Application
	{
	public:
		Application();
		virtual ~Application();

		virtual void init();
		virtual void doGameLoop() = 0;
		virtual void shutdown();
	};
}