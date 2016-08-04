// Ghostware Games inc. 2016  -Julian Williams
#pragma once

#include <iostream>
#include <chrono>
#include "s3eDebug.h"
#include "Types.h"
#include "StringHelper.h"
#include "s3eFile.h"

namespace GG
{
	class Log;
	enum class Level;
#ifdef _DEBUG
#define LOG_DEBUG(f_, ...)		Log::Tracef( GG::Log::Level::DEBUG, f_, __VA_ARGS__ )
#define LOG_INFO(f_, ...)		Log::Tracef( GG::Log::Level::INFO, f_, __VA_ARGS__ )
#define LOG_WARNING(f_, ...)	Log::Tracef( GG::Log::Level::WARNING, f_, __VA_ARGS__ )
#define LOG_ERROR(f_, ...)		Log::Tracef( GG::Log::Level::ERROR, f_, __VA_ARGS__ )
#else
#define LOG_DEBUG(f_, ...)
#define LOG_INFO(f_, ...)
#define LOG_WARNING(f_, ...)
#define LOG_ERROR(f_, ...)
#endif

	class Log
	{
	public:
		enum class Level
		{
			DEBUG	= 0 << 0,
			INFO	= 0 << 1,
			WARNING = 0 << 2,
			ERROR	= 0 << 3,

			ALL		= DEBUG | INFO | WARNING | ERROR,
		};




		static void Init( bool writeToFile = true )
		{
			if( !writeToFile ) {
				return;
			}

			auto now = std::chrono::system_clock::now();
			const std::time_t now_time = std::chrono::system_clock::to_time_t( now );
			auto t = localtime( &now_time );

			std::string logFilename;
			StringHelper::Format(
				logFilename,
				"ram://logs/log_%d-%d-%d_%dh-%dm-%ds.txt",
				t->tm_mon + 1,
				t->tm_hour + 1,
				t->tm_year + 1900,
				t->tm_hour,
				t->tm_min,
				t->tm_sec
			);

			_fileHandle = s3eFileOpen( logFilename.c_str(), "w" );
			IwAssert( LOGGING, _fileHandle != nullptr );
		}

		static void Shutdown()
		{
			if( _fileHandle != nullptr )
			{
				s3eFileFlush( _fileHandle );
				s3eFileClose( _fileHandle );
			}
		}

		static void SetLogLevel( uint logLevel )
		{
			_logLevel = logLevel;
		}

		template <typename ... Args>
		static void Tracef( Level logLevel, const char * format, const Args & ... args )
		{
			/*if( !( _logLevel & ( uint )logLevel ) ) {
				return;
			}*/

			std::string message;
			StringHelper::Format( message, format, StringHelper::Argument( args ) ... );

			if( _fileHandle != nullptr ) 
			{
				s3eFileWrite( message.data(), sizeof( char ), message.size() * sizeof( char ), _fileHandle );
				s3eFileFlush( _fileHandle );
			}

			s3eDebugOutputString( message.c_str() );
		}

	private:

		static uint			_logLevel;
		static s3eFile *	_fileHandle;
	};
}