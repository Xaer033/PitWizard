#pragma once

#include <string>
#include <GG/Core/IStream.h>

namespace GG
{
	namespace Stream
	{

		std::string		ReadStringFromStream(IStream * stream);

	}
}