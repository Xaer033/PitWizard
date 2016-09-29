#include "HashString.h"
#include <string>
#include <GG/Core/Types.h>

namespace GG
{
	uint32 hashString(const std::string & str)
	{
		return std::hash<std::string>{}(str);
	}
}