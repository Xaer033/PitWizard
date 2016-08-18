#pragma once

#include "ILogger.h"
#include <string>

struct s3eFile;

namespace GG
{
	class FileLogger : public ILogger
	{
	public:

		FileLogger( const std::string & logFilename );
		virtual ~FileLogger();

		virtual void init() final;
		virtual void write( const std::string & message ) final;
		virtual void shutdown() final;


	private:
		s3eFile *		_fileHandle;
		std::string		_filePath;
	};
}