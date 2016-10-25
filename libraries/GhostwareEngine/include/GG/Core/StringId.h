// Ghostware Games inc. 2016  -Julian Williams
#pragma once

#include "Types.h"
#include <string>

namespace GG
{
	typedef uint32 StringId;

	StringId			STRING_ID(const std::string & str);
	const std::string	GetStringFromId(const StringId & id);

}