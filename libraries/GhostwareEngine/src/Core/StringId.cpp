
#include "StringId.h"

#include <string>
#include <unordered_map>

#include <GG/Core/Types.h>
#include <GG/Core/Log.h>

namespace GG
{

	static std::unordered_map<StringId, std::string>	g_stringIdTable;

	//Raw hash function
	StringId	HashString(const std::string & str)
	{
		std::string key = str;
		std::transform(key.begin(), key.end(), key.begin(), ::tolower);

		return (StringId) std::hash<std::string>{}(key);
	}


	StringId	STRING_ID(const std::string & str)
	{
		StringId id = HashString(str);
		auto it = g_stringIdTable.find(id);
		if(it != g_stringIdTable.end())
		{
			TRACE_WARNING("String Id: %d already generated for string: %s", id, str);
			return it->first;
		}

		g_stringIdTable[id] = str;
		return id;

	}

	const std::string	GetStringFromId(const StringId & id)
	{
		auto it = g_stringIdTable.find(id);
		if(it == g_stringIdTable.end())
		{
			TRACE_WARNING("String id: %d was not generated at runtime, I don't have it's original String value", id);
			return "null";
		}
		return it->second;
	}
}