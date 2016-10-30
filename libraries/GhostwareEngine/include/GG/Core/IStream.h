// Ghostware Games inc. 2016  -Julian Williams
#pragma once

#include "Types.h"

namespace GG
{

	class IStream
	{
	public:
		virtual ~IStream() {}

		virtual int64	read(void* buffer, uint32 size, uint32 count)	= 0;
		virtual int64	write(const void* buffer, uint32 size, uint32 count) = 0;
		virtual int64	seek(uint64 position) = 0;
		virtual int64	tell() const = 0;
		virtual uint64	getSize() const	= 0;
	};
}