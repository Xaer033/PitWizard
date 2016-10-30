// Ghostware Games inc. 2016  -Julian Williams
#pragma once
#include <string>
#include "JsonLib/json.h"

namespace GG
{
	typedef Json::Value json;

	// *TODO use Resource Manager
	json JsonFromFile( const std::string & filename );
	json JsonFromString(const std::string & str);
	
}