// Ghostware Games inc. 2016  -Julian Williams
#pragma once

#include <string>
#include "IwDebug.h"

namespace GG
{
	namespace StringHelper
	{

// ---- Custom Arguments that can be expanded on by other sub systems ----
		template <typename T>
		T Argument( T value ) 
		{
			return value;
		}

		template <typename T>
		T const * Argument( const std::basic_string<T>  & value ) 
		{
			return value.c_str();
		}
// ------------------------------------------------------------------------
		

		template <typename T, typename ... Args>
		void Format( 
			std::basic_string<T> & buffer,
			const T *  format,
			const Args & ... args )
		{
			const size_t size = _implPrint(
				&buffer[ 0 ],
				buffer.size() + 1,
				format,
				args ... );

			if( size > buffer.size() )
			{
				buffer.resize( size );
				_implPrint( &buffer[ 0 ], buffer.size() + 1, format, args ... );
			}
			else if( size < buffer.size() )
			{
				buffer.resize( size );
			}
		}

		template <typename ... Args>
		int _implPrint(
			char * buffer,
			const size_t bufferCount,
			const char * format, const Args & ... args )
		{
			const int result = snprintf( 
				buffer,
				bufferCount,
				format,
				Argument( args ) ... );

			IwAssert( STRING_HELPER, -1 != result );
			return result;
		}


	}
}