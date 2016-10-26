// Ghostware Games inc. 2016  -Julian Williams
#pragma once 

namespace GG
{
	class IApplication
	{
	public:
		virtual ~IApplication() {};

		virtual void init(int argc, char** argv) = 0;
		virtual void doGameLoop() = 0;
		virtual void shutdown() = 0;
	};
}