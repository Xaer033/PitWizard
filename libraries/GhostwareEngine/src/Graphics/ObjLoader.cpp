// Ghostware Games inc. 2016  -Julian Williams

#include "ObjLoader.h"

#include <iostream>
#include <vector>
#include <fstream>
#include <stdio.h>
#include <string>

#include <GG/Core/Vector.h>
#include <GG/Core/Log.h>
#include <GG/Graphics/StaticGeometryBuffer.h>

namespace GG
{

	bool ObjLoader::loadFromFile(const std::string & fileLocation, StaticGeometryBuffer & mesh)
	{
		mesh.clearVertices();

		const uint kMaxLineSize = 256;


		std::vector< uint	>	indexList;
	
		std::vector< Vector3 >	vertPosList;
		std::vector< Vector2 >	texCoordList;
		std::vector< Vector3 >	normalList;
	
	
		FILE * gFile = fopen( fileLocation.c_str(), "r" );
	
		if ( gFile == nullptr ) 
		{ 
			LOG_ERROR("File '%s' could not be opened!", fileLocation);
			return false; 
		}

		char line[ kMaxLineSize ]; 
	 
	
		int v  = 0; //vert pos
		int vt = 0; //vert texCoord
		int vn = 0; //vert Normal
	
	
		Vector3 min, max;
	//Pass 1
		int lineCount = 0;
	
		while( fgets( line, 128, gFile ) != NULL )
	    {
			lineCount++;
	
			
	        switch( line[0] )
	        {
			case '#':
			case 's':
			case 'm':
			case 'u':
				continue;
	//Handle Vertices
	        case 'v':   // v, vt, or vn
	            switch( line[1] )
	            {
			//Position
	            case ' ':
					{
					Vector3 v;
	                sscanf(line, "v %f %f %f", &v.x, &v.y, &v.z);
	                
					min.x = (v.x < min.x) ? v.x : min.x;
					min.y = (v.y < min.y) ? v.y : min.y;
					min.z = (v.z < min.z) ? v.z : min.z;
	
					max.x = (v.x > max.x) ? v.x : max.x;
					max.y = (v.y > max.y) ? v.y : max.y;
					max.z = (v.z > max.z) ? v.z : max.z;
	
					vertPosList.push_back( v );
	
					}
	                break;
			//Normals
	            case 'n':{
	                Vector3 vn;
	                sscanf(line, "vn %f %f %f", &vn.x, &vn.y, &vn.z);
	             
					normalList.push_back( vn );
					}
	                break;
			//UV Tex Coords
	            case 't':
					{
	                Vector2 vt;
	                sscanf(line, "vt %f %f", &vt.x, &vt.y);
	                
					texCoordList.push_back( vt );
					}
	
	            default:
	                break;
	            }
	            break;
	
	
		//Handle Faces
			case 'f':
				{
				//std::cout << line << std::endl;
					unsigned int posIndex[3];
					unsigned int texIndex[3];
					unsigned int normalIndex[3];
	
				//Position
					sscanf( line, "f %d/%d/%d %d/%d/%d %d/%d/%d",	&posIndex[0], &texIndex[0], &normalIndex[0],
																	&posIndex[1], &texIndex[1], &normalIndex[1],
																	&posIndex[2], &texIndex[2], &normalIndex[2] );
				
					
					for( uint i = 0; i < 3; ++i )
					{
						mesh.pushTexCoord( texCoordList[ texIndex[i] - 1 ]	);
						mesh.pushNormal( normalList[ normalIndex[i]	- 1 ]	);
						mesh.addPosition( vertPosList[ posIndex[i] - 1	]	);
					}
				}
	            break;

	        default:
	            break;
			}
		}

		fclose ( gFile );
	
		//mesh.aaBoundingBox.min = min;
		//mesh.aaBoundingBox.max = max;
	
		
		mesh.build( );
	
		return true;
	}
	
	
	
	bool ObjLoader::loadFromMemory(char * info, uint size, StaticGeometryBuffer & mesh)
	{
	
		return true;
	}
	
	
	bool ObjLoader::saveToFile( const std::string & fileLocation, StaticGeometryBuffer & mesh)
	{
	
		return true;
	}

}