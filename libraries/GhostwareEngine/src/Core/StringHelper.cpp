#include "StringHelper.h"
#include <string>

namespace GG
{
	namespace StringHelper
	{
		void RemovePath( const std::string & in, std::string & out )
		{
			out = in;
			const size_t lastSlashIndex = out.find_last_of( "\\/" );
			if( std::string::npos != lastSlashIndex )
			{
				out.erase( 0, lastSlashIndex + 1 );
			}
		}
	}
}

