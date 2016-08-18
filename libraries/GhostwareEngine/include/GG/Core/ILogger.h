#pragma once

#include <string>

namespace GG
{
	class ILogger
	{
	public:
		ILogger() {}
		virtual ~ILogger() {}

		virtual void init() = 0;
		virtual void write( const std::string & message ) = 0;
		virtual void shutdown() = 0;

	};
}