
#include "StreamVertexBuffer.h"

#include <IwGL.h>
#include <GG/Core/Vector.h>

namespace GG
{
	using namespace Graphics;

    StreamVertexBuffer::StreamVertexBuffer() : 
        IVertexBuffer(),    _currentUv0(),     _currentNormal(), 
        _currentTangent(), _currentBitangent(),     _currentColor()
    {
    }

    StreamVertexBuffer::~StreamVertexBuffer()
    {
        clearVertices();
        _destroy();
    }

    void StreamVertexBuffer::clearVertices()
    {
        _positionList.clear();
        _uv0List.clear();
        _normalList.clear();
        _tangentList.clear();
        _biTangentList.clear();
        _colorList.clear();
    }

    void StreamVertexBuffer::addPosition( const Vector3 & p )
    {
        //Posistion is always required
        _positionList.push_back( p );

        int vertProperty = getVertexProperties();

        if( vertProperty & GG::UV0 )
            _uv0List.push_back( _currentUv0 );
        
        if( vertProperty & GG::NORMALS )
            _normalList.push_back( _currentNormal );
        
        if( vertProperty & GG::TANGENTS )
            _tangentList.push_back( _currentTangent );
        
        if( vertProperty & GG::BITANGENTS )
            _biTangentList.push_back( _currentBitangent );
        
        if( vertProperty & GG::COLORS )
            _colorList.push_back( _currentColor );
        
    }

    void StreamVertexBuffer::pushUv0( const Vector2 & uv0 )
    {
        _currentUv0 = uv0;
    }

	void StreamVertexBuffer::pushUv1(const Vector2 & uv1)
	{
		_currentUv1 = uv1;
	}

    void StreamVertexBuffer::pushNormal( const Vector3 & normal )
    {
        _currentNormal = normal;
    }

    void StreamVertexBuffer::pushColor( const Vector4 & color )
    {
        _currentColor = color;
    }

    void StreamVertexBuffer::pushTangent( const Vector3 & tangent )
    {
        _currentTangent = tangent;
    }

    void StreamVertexBuffer::pushBitangent( const Vector3 & biTangent )
    {
        _currentBitangent = biTangent;
    }

	void StreamVertexBuffer::pushIndex(uint index)
	{
		
	}

    void StreamVertexBuffer::build( const DrawHint drawHint )
    {

    }

    void StreamVertexBuffer::merge( const StreamVertexBuffer & buffer )
    {
        _positionList.insert(   _positionList.end(),    buffer._positionList.begin(),   buffer._positionList.end()  );
        _uv0List.insert(		_uv0List.end(),			buffer._uv0List.begin(),   buffer._uv0List.end()  );
        _uv1List.insert(		_uv1List.end(),			buffer._uv1List.begin(),   buffer._uv1List.end()  );
		_normalList.insert(     _normalList.end(),      buffer._normalList.begin(),     buffer._normalList.end()    );
        _tangentList.insert(    _tangentList.end(),     buffer._tangentList.begin(),    buffer._tangentList.end()   );
        _biTangentList.insert(  _biTangentList.end(),   buffer._biTangentList.begin(),  buffer._biTangentList.end() );
        _colorList.insert(      _colorList.end(),       buffer._colorList.begin(),      buffer._colorList.end()     );
    }

    void StreamVertexBuffer::bind() const
    {
		int Position = VertexAttribute::GetIndex(VertexProperty::POSITIONS);
        glEnableVertexAttribArray(Position);
        glVertexAttribPointer(Position, 3, GL_FLOAT, GL_FALSE, sizeof( Vector3 ), &_positionList[ 0 ] );

        int vertexProperties = getVertexProperties();


		int Uv0 = VertexAttribute::GetIndex(VertexProperty::UV0);
        if( vertexProperties & VertexProperty::UV0 )
        {
            glEnableVertexAttribArray(Uv0);
            glVertexAttribPointer(Uv0, 2, GL_FLOAT, GL_FALSE, sizeof( Vector2 ), &_uv0List[ 0 ] );
        }
        else
        {
            glDisableVertexAttribArray(Uv0);
        }

		int Uv1 = VertexAttribute::GetIndex(VertexProperty::UV1);
		if(vertexProperties & VertexProperty::UV1)
		{
			glEnableVertexAttribArray(Uv1);
			glVertexAttribPointer(Uv1, 2, GL_FLOAT, GL_FALSE, sizeof(Vector2), &_uv1List[0]);
		}
		else
		{
			glDisableVertexAttribArray(Uv1);
		}


		int Normal = VertexAttribute::GetIndex(VertexProperty::NORMALS);
        if( vertexProperties & VertexProperty::NORMALS )
        {
            glEnableVertexAttribArray(Normal);
            glVertexAttribPointer(Normal, 3, GL_FLOAT, GL_FALSE, sizeof( Vector3 ), &_normalList[ 0 ] );
        }
        else
        {
            glDisableVertexAttribArray(Normal);
        }


		int Tangent = VertexAttribute::GetIndex(VertexProperty::TANGENTS);
        if( vertexProperties & VertexProperty::TANGENTS )
        {
            glEnableVertexAttribArray(Tangent);
            glVertexAttribPointer(Tangent, 3, GL_FLOAT, GL_FALSE, sizeof( Vector3 ), &_tangentList[ 0 ] );
        }
        else
        {
            glDisableVertexAttribArray( Tangent );
        }


		int Bitangent = VertexAttribute::GetIndex(VertexProperty::BITANGENTS);
        if( vertexProperties & VertexProperty::BITANGENTS )
        {
            glEnableVertexAttribArray( Bitangent );
            glVertexAttribPointer( Bitangent, 3, GL_FLOAT, GL_FALSE, sizeof( Vector3 ), &_biTangentList[ 0 ] );
        }
        else
        {
            glDisableVertexAttribArray( Bitangent );
        }

		int Color = VertexAttribute::GetIndex(VertexProperty::COLORS);
        if( vertexProperties & VertexProperty::COLORS )
        {
            glEnableVertexAttribArray( Color );
            glVertexAttribPointer( Color, 4, GL_FLOAT, GL_FALSE, sizeof( Vector4 ), &_colorList[ 0 ] );
        }
        else
        {
            glDisableVertexAttribArray( Color );
        }
    }

    void StreamVertexBuffer::render( const DrawMode & drawMode ) const
    {
        glDrawArrays( ( GLenum )drawMode, 0, _positionList.size() );
    }

    //---------------------------- Private Implementation ----------------------------
    void StreamVertexBuffer::_destroy()
    {
    }
}