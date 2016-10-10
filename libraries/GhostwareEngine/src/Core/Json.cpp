
#include "Json.h"
#include <string>
#include <sstream>
#include <fstream>

namespace GG
{
	json JsonFromString(const std::string & str)
	{
		std::stringstream buffer;
		buffer << str;

		Json::CharReaderBuilder builder;
		builder["collectComments"]	= true;
		builder["allowComments"]	= true;
		builder["allowNumericKeys"]	= true;

		json value = json::null;
		std::string errs;
		bool ok = Json::parseFromStream(builder, buffer, &value, &errs);

		return value;
	}

	json JsonFromFile( const std::string & filename )
	{
		std::ifstream file( filename );
		std::stringstream buffer;
		buffer << file.rdbuf();

		Json::CharReaderBuilder builder;
		builder[ "collectComments" ]	= true;
		builder[ "allowComments" ]		= true;
		builder[ "allowNumericKeys" ]	= true;

		json value = json::null;
		std::string errs;
		bool ok = Json::parseFromStream( builder, buffer, &value, &errs );

		return value;
	}
}