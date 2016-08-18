#pragma once
#include <string>
#include "JsonLib\json.h"

namespace GG
{
	typedef Json::Value json;

	// *TODO use Resource Manager
	json JsonFromFile( const std::string & filename );
	
}