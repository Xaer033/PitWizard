// Ghostware Games inc. 2016  -Julian Williams
#pragma once

#include <string>


namespace GG
{
	class StaticGeometryBuffer;

	class ObjLoader
	{
	public: 

		static bool loadFromFile( const std::string & fileLocation , StaticGeometryBuffer & mesh );
		static bool loadFromMemory( char * info, unsigned int size, StaticGeometryBuffer & mesh );
		static bool saveToFile( const std::string & fileLocation, StaticGeometryBuffer & mesh );

	};
}
