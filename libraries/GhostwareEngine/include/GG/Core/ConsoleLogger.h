#pragma once

#include "ILogger.h"
#include <string>
#include <s3eDebug.h>

namespace GG
{
	class ConsoleLogger : public ILogger
	{
	public:

		virtual void init() final {}

		virtual void write( const std::string & message ) final
		{
			s3eDebugOutputString( message.c_str() );
		}

		virtual void shutdown() final {}
	};
}