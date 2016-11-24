// Ghostware Games inc. 2016  -Julian Williams
#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <sstream>

#include "ILogger.h"
#include "s3eDebug.h"
#include "Types.h"
#include "StringHelper.h"


namespace GG
{
	class Log;
	enum class Level;
//
//#ifdef _DEBUG
#define LOG_DEBUG(f_, ...)		GG::Log::Get()->logf( GG::Log::Level::DEBUG,	f_, ##__VA_ARGS__ )
#define LOG_INFO(f_, ...)		GG::Log::Get()->logf( GG::Log::Level::INFO,		f_, ##__VA_ARGS__ )
#define LOG_WARNING(f_, ...)	GG::Log::Get()->logf( GG::Log::Level::WARNING,	f_, ##__VA_ARGS__ )
#define LOG_ERROR(f_, ...)		GG::Log::Get()->logf( GG::Log::Level::ERROR,	f_, ##__VA_ARGS__ )

#define TRACE_DEBUG(f_, ...)	GG::Log::Get()->tracef( GG::Log::Level::DEBUG,		__FILE__, __LINE__,	f_, ##__VA_ARGS__ )
#define TRACE_INFO(f_, ...)		GG::Log::Get()->tracef( GG::Log::Level::INFO,		__FILE__, __LINE__,	f_, ##__VA_ARGS__ )
#define TRACE_WARNING(f_, ...)	GG::Log::Get()->tracef( GG::Log::Level::WARNING,	__FILE__, __LINE__,	f_, ##__VA_ARGS__ )
#define TRACE_ERROR(f_, ...)	GG::Log::Get()->tracef( GG::Log::Level::ERROR,		__FILE__, __LINE__,	f_, ##__VA_ARGS__ )

//#else
//#define LOG_DEBUG(f_, ...)
//#define LOG_INFO(f_, ...)
//#define LOG_WARNING(f_, ...)
//#define LOG_ERROR(f_, ...)
//
//#define TRACE_DEBUG(f_, ...)	
//#define TRACE_INFO(f_, ...)		
//#define TRACE_WARNING(f_, ...)	
//#define TRACE_ERROR(f_, ...)
//#endif

	class ILog
	{
	public:
	};

	class Log
	{
	public:
		enum class Level
		{
			ALL		= 0,
			DEBUG,
			INFO,
			WARNING,
			ERROR,
		};

		static Log * Get()
		{
			if( _instance == nullptr )
			{
				_instance = new Log();
			}
			return _instance;
		}

		static void Shutdown()
		{
			if( _instance != nullptr )
			{
				delete _instance;
				_instance = nullptr;
			}
		}
		
		~Log()
		{
			unregisterAllLoggers();
		}

		void registerLogger( ILogger * logger )
		{
			_loggerList.push_back( std::unique_ptr<ILogger>( logger ) );
			_loggerList.back()->init();

		}

		void unregisterAllLoggers()
		{
			for( auto && logger : _loggerList )
			{
				logger->shutdown();
			}

			_loggerList.clear();
		}

		void setLogLevel( Level logLevel )
		{
			_logLevel = (uint)logLevel;
		}

		template <typename ... Args>
		void logf( 
			Level logLevel, 
			const char * format, 
			const Args & ... args )
		{
			if( !_sanityCheck( logLevel ) )
			{
				return;
			}

			StringHelper::Format( 
				_messageBuffer, 
				format, 
				StringHelper::Argument( args ) ... );

			for( auto && logger : _loggerList )
			{
				logger->write( _messageBuffer );
			}
		}

		template <typename ... Args>
		void tracef( 
			Level logLevel, 
			const char* filePath, 
			uint line, 
			const char *format, 
			const Args & ... args )
		{
			if( !_sanityCheck( logLevel ) )
			{
				return;
			}

			StringHelper::RemovePath( filePath, _messageBuffer );

			_prefix.str( _getLevelString( logLevel ));
			_prefix << _messageBuffer << "-" << line << ": ";

			StringHelper::Format( 
				_messageBuffer, 
				format, 
				StringHelper::Argument( args ) ... );

			_prefix << _messageBuffer << std::endl;

			for( auto && logger : _loggerList )
			{
				logger->write( _prefix.str() );
			}
		}

	private:
		Log() :_logLevel((uint)Level::ALL)
		{
		}



		const std::string _getLevelString( Level logLevel )
		{
			switch( logLevel )
			{
			case Level::INFO:		return "Info - ";
			case Level::DEBUG:		return "Debug - ";
			case Level::WARNING:	return "Warning - ";
			case Level::ERROR:		return "Error - ";
			}

			return "N/A";
		}


		bool _sanityCheck( Level level )
		{
			if( ( uint )level < _logLevel )
			{
				return false;
			}

			if( _loggerList.empty() )
			{
				std::cout << "No Loggers Registered!" << std::endl;
				return false;
			}

			return true;
		}

		static Log * _instance;

		uint _logLevel;
		std::vector< std::unique_ptr<ILogger> >	_loggerList;

		std::string _messageBuffer;
		std::stringstream _prefix;
	};
}