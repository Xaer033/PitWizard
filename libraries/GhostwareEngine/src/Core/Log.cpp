#include "Log.h"

namespace GG
{ 
	uint			Log::_logLevel	= ( uint )Level::ALL;
	s3eFile *	Log::_fileHandle = nullptr;
}