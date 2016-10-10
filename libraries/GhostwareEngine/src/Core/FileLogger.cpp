
#include "FileLogger.h"
#include "IwDebug.h"
#include "s3eFile.h"


namespace GG
{
	FileLogger::FileLogger( const std::string & logFilename ) 
	{
		_filePath = logFilename;
		_fileHandle = nullptr;
	}
	FileLogger::~FileLogger()
	{
	}

	void FileLogger::init()
	{
		_fileHandle = s3eFileOpen( _filePath.c_str(), "w" );
		IwAssert( LOGGING, _fileHandle != nullptr );
	}

	void FileLogger::write( const std::string & message )
	{
		static const size_t kCharSize = sizeof( char );

		if( _fileHandle == nullptr ) {
			return;
		}

		s3eFileWrite( 
			message.data(), 
			kCharSize,
			message.size() * kCharSize,
			_fileHandle 
		);

		s3eFileFlush( _fileHandle );
	}

	void FileLogger::shutdown()
	{
		if( _fileHandle == nullptr ) {
			return;
		}

		s3eFileFlush( _fileHandle );
		s3eFileClose( _fileHandle );
		_fileHandle = nullptr;
	}
}