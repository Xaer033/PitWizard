
#include "Json.h"
#include <string>
#include <sstream>
#include <GG/Core/FileStream.h>
#include <GG/Core/StreamHelper.h>
#include <GG/Core/Log.h>

namespace GG
{
	json JsonFromString(const std::string & str)
	{
		Json::CharReaderBuilder builder;
		builder["collectComments"]	= true;
		builder["allowComments"]	= true;
		builder["allowNumericKeys"]	= true;

		std::stringstream buffer;
		buffer << str;

		json value = json::null;
		std::string errs;
		const char * cBuff = str.c_str();
		bool ok = Json::parseFromStream(builder, buffer, &value, &errs);
		if(!ok)
		{
			TRACE_ERROR("Json Error: %s", errs);
			return json::nullRef;
		}
		return value;
	}

	json JsonFromFile( const std::string & filename )
	{
		FileStream stream(filename, OpenMode::OPEN_READ);
		if(!stream.isOpen())
		{
			TRACE_WARNING("Could not open json file from: %s", filename);
			return json::nullRef;
		}
		return JsonFromString(Stream::ReadStringFromStream(&stream));
	}
}