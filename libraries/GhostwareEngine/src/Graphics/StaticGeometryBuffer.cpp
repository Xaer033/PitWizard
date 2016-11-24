/*
	Module: Geometry Buffer
	Description: Geometry that we send to the graphics card
	Author: Julian Williams
*/

#include "StaticGeometryBuffer.h"

#include "IVertexBuffer.h"

#include <vector>
#include <cmath>
#include <sstream>

#include <IwGL.h>

#include <GG/Core/Vector.h>
#include <GG/Core/MathDebug.h>
#include <GG/Core/Log.h>


#define BUFFER_OFFSET(i) ((void*)(i))


namespace GG
{
	StaticGeometryBuffer::StaticGeometryBuffer()  :
		_vertexBufferHandle(0), 
        _indexBufferHandle(0),  
		_arrayBufferHandle(0)
	{
		_vertexProperties = 0;
		_vertexList.clear();
		_indexList.clear();
	}

	StaticGeometryBuffer::~StaticGeometryBuffer()
	{
		if( _vertexBufferHandle != 0 )
            glDeleteBuffers( 1, &_vertexBufferHandle );

        if( _indexBufferHandle != 0 )
            glDeleteBuffers( 1, &_indexBufferHandle );
	}


    void StaticGeometryBuffer::merge( const StaticGeometryBuffer & buffer )
	{
		_vertexList.insert	( _vertexList.end(),	buffer._vertexList.begin(),	buffer._vertexList.end()	);
		_indexList.insert	( _indexList.end(),		buffer._indexList.begin(),	buffer._indexList.end()		);
	}


    void StaticGeometryBuffer::clearVertices()
	{
		_vertexList.clear();
		_indexList.clear();
	}

    void StaticGeometryBuffer::addPosition( const Vector3 & p )
	{
		Vertex v = 
		{ 
			p, 
			_currentTexCoord, 
			_currentNormal,  
			_currentTangent, 
			_currentBitangent,
			_currentColor
		};
	
		_vertexList.push_back( v );

	}

    void StaticGeometryBuffer::pushTexCoord( const Vector2 & t )
	{
		_currentTexCoord = t;
	}

    void StaticGeometryBuffer::pushNormal( const Vector3 & n )
	{
		_currentNormal = n;
	}

    void StaticGeometryBuffer::pushColor( const Vector4 & c )
	{
		_currentColor = c;
	}

    void StaticGeometryBuffer::pushTangent( const Vector3 & t )
	{
		_currentTangent = t;
	}

    void StaticGeometryBuffer::pushBitangent( const Vector3 & b )
	{
		_currentBitangent = b;
	}


	void StaticGeometryBuffer::pushIndex(uint index)
	{
		_indexList.push_back(index);
	}

    void StaticGeometryBuffer::_generateTangents()
	{
		Vertex v[3];

		for( uint i = 0; i < _vertexList.size(); i += 3 )
		{
			if( i >= _vertexList.size() - 3 )
				break;
	
		
			v[0] = _vertexList[i];
			v[1] = _vertexList[i+1];
			v[2] = _vertexList[i+2];

			Vector3 tangent, biTangent;

	//Make Tangent/bitangent building
			Vector3 edge1	= v[1].position - v[0].position;
			Vector3 edge2	= v[2].position - v[0].position;


			Vector2 edge1uv = v[1].texCoord - v[0].texCoord;
			Vector2 edge2uv = v[2].texCoord - v[0].texCoord;

			//Determ
			float det = edge1uv.x * edge2uv.y - edge1uv.y * edge2uv.x;

			if(det != 0.0f) 
			{
				float mul = 1.0f / det;
				tangent	  = (edge1 * -edge2uv.y + edge2 * edge1uv.y) * mul;
				biTangent = (edge1 * -edge2uv.x + edge2 * edge1uv.x) * mul;
			}


			const float kEpsilon = 0.001f;
			if( fabsf( tangent.x ) < kEpsilon && 
				fabsf( tangent.y ) < kEpsilon &&
				fabsf( tangent.z ) < kEpsilon)
			{
				TRACE_WARNING("Tangent vector has length of 0");
			}

			for( uint j = 0; j < 3; ++j )
			{
				_vertexList[i + j].tangent		= tangent;
				_vertexList[i + j].biTangent	= biTangent;
			}
		}
	}

    
    void StaticGeometryBuffer::build( DrawHint drawHint )
	{
		/*if(getVertexProperties() & TANGENTS)
			_generateTangents();

		_generateIndices();
		*/
		
        if( _vertexBufferHandle == 0 )
            glGenBuffers( 1, &_vertexBufferHandle );

        if( _indexBufferHandle == 0 )
            glGenBuffers( 1, &_indexBufferHandle );
		
        int vCount = getVertexCount();
        int iCount = getIndexCount();

        glBindBuffer( GL_ARRAY_BUFFER,          _vertexBufferHandle );
        glBufferData( GL_ARRAY_BUFFER,          sizeof( Vertex ) * _vertexList.size(), &_vertexList[ 0 ], drawHint );
        //glBufferSubData( GL_ARRAY_BUFFER, 0,    sizeof( Vertex ) * _vertexList.size(), &_vertexList[ 0 ] );


        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER,          _indexBufferHandle );
        glBufferData( GL_ELEMENT_ARRAY_BUFFER,          sizeof( GLuint ) * _indexList.size(), &_indexList[ 0 ] , drawHint );
       // glBufferSubData( GL_ELEMENT_ARRAY_BUFFER, 0,    sizeof( GLuint ) * _indexList.size(), &_indexList[ 0 ] );
	
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);


	}

    void StaticGeometryBuffer::build( 
		Vertex *	vertexList,   
		uint		vertCount,
        uint *		indexList,   
		uint		indexCount,
        DrawHint	drawHint )
	{

		_vertexList.clear();

		for( uint i = 0; i < vertCount; ++i )
			_vertexList.push_back( vertexList[ i ] );


        if( indexList == nullptr || indexCount == 0 )
        {
            _generateIndices();
        }
		else
		{
			_indexList.clear();

			for( uint i = 0; i < indexCount; ++i )
				_indexList.push_back( indexList[ i ] );
		}

		
		if( getVertexProperties() & TANGENTS )
			_generateTangents();

        if( _vertexBufferHandle == 0 )
            glGenBuffers( 1, &_vertexBufferHandle );

        if( _indexBufferHandle == 0 )
            glGenBuffers( 1, &_indexBufferHandle );
		

        glBindBuffer( GL_ARRAY_BUFFER,          _vertexBufferHandle );
        glBufferData( GL_ARRAY_BUFFER,          sizeof( Vertex ) * _vertexList.size(), NULL, drawHint   );
		glBufferSubData( GL_ARRAY_BUFFER, 0,    sizeof( Vertex ) * _vertexList.size(), &_vertexList[0]  );


        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, _indexBufferHandle );
		glBufferData( GL_ELEMENT_ARRAY_BUFFER,          sizeof( GLuint ) * _indexList.size(), NULL, drawHint    );
        glBufferSubData( GL_ELEMENT_ARRAY_BUFFER, 0,    sizeof( GLuint ) * _indexList.size(), &_indexList[ 0 ]  );
	}

    void StaticGeometryBuffer::bind() const
    {
        glBindBuffer( GL_ARRAY_BUFFER,          _vertexBufferHandle );
        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER,  _indexBufferHandle  );

        glEnableVertexAttribArray( VertexTags::Position );
        glVertexAttribPointer(VertexTags::Position, 3, GL_FLOAT, GL_FALSE, sizeof( Vertex ), BUFFER_OFFSET( 0 ) );
			
        int vertexProperties = getVertexProperties();

        if( vertexProperties & GG::UV0 )
        {
            glEnableVertexAttribArray(VertexTags::Uv0);
            glVertexAttribPointer(VertexTags::Uv0, 2, GL_FLOAT, GL_FALSE, sizeof( Vertex ), BUFFER_OFFSET( 3 * sizeof( float ) ) );
        }
        else
        {
            glDisableVertexAttribArray(VertexTags::Uv0 );
        }

        if( vertexProperties & GG::NORMALS )
        {
            glEnableVertexAttribArray(VertexTags::Normal );
            glVertexAttribPointer(VertexTags::Normal, 3, GL_FLOAT, GL_FALSE, sizeof( Vertex ), BUFFER_OFFSET( 5 * sizeof( float ) ) );
        }
        else
        {
            glDisableVertexAttribArray(VertexTags::Normal );
        }

        if( vertexProperties & GG::TANGENTS )
        {
            glEnableVertexAttribArray(VertexTags::Tangent );
            glVertexAttribPointer(VertexTags::Tangent, 3, GL_FLOAT, GL_FALSE, sizeof( Vertex ), BUFFER_OFFSET( 8 * sizeof( float ) ) );
        }
        else
        {
            glDisableVertexAttribArray(VertexTags::Tangent );
        }

        if( vertexProperties & GG::BITANGENTS )
        {
            glEnableVertexAttribArray(VertexTags::Bitangent );
            glVertexAttribPointer(VertexTags::Bitangent, 3, GL_FLOAT, GL_FALSE, sizeof( Vertex ), BUFFER_OFFSET( 11 * sizeof( float ) ) );
        }
        else
        {
            glDisableVertexAttribArray(VertexTags::Bitangent );
        }

        if( vertexProperties & GG::COLORS )
        {
            glEnableVertexAttribArray(VertexTags::Color );
            glVertexAttribPointer(VertexTags::Color, 4, GL_FLOAT, GL_FALSE, sizeof( Vertex ), BUFFER_OFFSET( 14 * sizeof( float ) ) );
        }
        else
        {
            glDisableVertexAttribArray(VertexTags::Color );
        }
    }

    void StaticGeometryBuffer::render( const DrawMode & drawMode ) const
    {
		/*for(auto &v : _vertexList)
		{
			TRACE_DEBUG("V: %s %s %s", ToString(v.position), ToString(v.texCoord), ToString(v.normal));
		}*/
		glDrawElements((GLenum)drawMode, _indexList.size(), GL_UNSIGNED_INT, 0);// (void*)(sizeof(uint) * (_indexList.size() / 2)) );
	}

	void StaticGeometryBuffer::render(const DrawMode & drawMode, uint startIndex, uint indexCount) const
	{
		glDrawElements((GLenum)drawMode, indexCount, GL_UNSIGNED_INT, (void*)(sizeof(uint) * startIndex) );
	}


	const StaticGeometryBuffer::Vertex *   StaticGeometryBuffer::getVertexArray() const
    {
        return &_vertexList[ 0 ];
    }

    const uint *    StaticGeometryBuffer::getIndexArray() const
    {
        return &_indexList[ 0 ];
    }

    uint  StaticGeometryBuffer::getVertexCount() const
    {
        return _vertexList.size();
    }

    uint  StaticGeometryBuffer::getIndexCount() const
    {
        return _indexList.size();
    }
//---------------------------- Private Implementation -------------------------------------

	///check 2 verticies to see if they're the same
    bool StaticGeometryBuffer::_isSameVertex( const Vertex & v1, const Vertex & v2 ) const
	{
		const float kLimit = 0.001f;

		Vector3 deltaPos	= v1.position - v2.position;
		bool posResult		= (	fabsf( deltaPos.x ) < kLimit && 
								fabsf( deltaPos.y ) < kLimit &&
								fabsf( deltaPos.z ) < kLimit	);

        int vertexProperties = getVertexProperties();

		bool texResult = true;
		if( vertexProperties & GG::UV0 )
		{
			Vector2 deltaTex = v1.texCoord - v2.texCoord;
			texResult = (fabsf(deltaTex.x) < kLimit &&
						 fabsf(deltaTex.y) < kLimit);
		}

		bool normalResult = true;
		if( vertexProperties & GG::NORMALS )
		{
			Vector3 deltaNormal = v1.normal - v2.normal;
			normalResult = (fabsf(deltaNormal.x) < kLimit &&
							fabsf(deltaNormal.y) < kLimit &&
							fabsf(deltaNormal.z) < kLimit);
		}
		
		bool colorResult = true;
		if( vertexProperties & GG::COLORS )
		{
			Vector4 deltaColor = v1.color - v2.color;
			colorResult =  (fabsf(deltaColor.x) < kLimit &&
							fabsf(deltaColor.y) < kLimit &&
							fabsf(deltaColor.z) < kLimit &&
							fabsf(deltaColor.w) < kLimit);
		}

		bool tangentResult = true;
		if( vertexProperties & GG::TANGENTS )
		{
			Vector3 deltaTangent = v1.tangent - v2.tangent;
			tangentResult = (fabsf(deltaTangent.x) < kLimit &&
							 fabsf(deltaTangent.y) < kLimit &&
							 fabsf(deltaTangent.z) < kLimit);
		}

		return posResult && texResult && normalResult && colorResult && tangentResult;
	}

    void StaticGeometryBuffer::_generateIndices()
    {
        _indexList.clear();

        VertexList  newVertList;
        //DebugText   debugText;

        for( uint i = 0; i < _vertexList.size(); ++i )
        {
            bool repeated = false;

            ///FLip uv's for openGL
            _vertexList[ i ].texCoord.y = 1 - _vertexList[ i ].texCoord.y;

            for( uint j = 0; j < newVertList.size(); ++j )
            {

                repeated = _isSameVertex( _vertexList[ i ], newVertList[ j ] );

                if( repeated )
                {
                    _indexList.push_back( j );
                    break;
                }
            }


            if( !repeated )
            {
                newVertList.push_back( _vertexList[ i ] );
                _indexList.push_back( newVertList.size() - 1 );
            }
        }

        _vertexList = newVertList;
    }
}
