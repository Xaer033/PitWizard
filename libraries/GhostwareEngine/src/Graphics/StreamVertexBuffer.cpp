
#include "StreamVertexBuffer.h"

#include <IwGL.h>
#include <GG/Core/Vector.h>

namespace GG
{
    StreamVertexBuffer::StreamVertexBuffer() : 
        IVertexBuffer(),    _currentTexCoord(),     _currentNormal(), 
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
        _texCoordList.clear();
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

        if( vertProperty & GG::TEXCOORDS )
            _texCoordList.push_back( _currentTexCoord );
        
        if( vertProperty & GG::NORMALS )
            _normalList.push_back( _currentNormal );
        
        if( vertProperty & GG::TANGENTS )
            _tangentList.push_back( _currentTangent );
        
        if( vertProperty & GG::BITANGENTS )
            _biTangentList.push_back( _currentBitangent );
        
        if( vertProperty & GG::COLORS )
            _colorList.push_back( _currentColor );
        
    }

    void StreamVertexBuffer::pushTexCoord( const Vector2 & texCoord )
    {
        _currentTexCoord = texCoord;
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

    void StreamVertexBuffer::build( const DrawHint drawHint )
    {

    }

    void StreamVertexBuffer::merge( const StreamVertexBuffer & buffer )
    {
        _positionList.insert(   _positionList.end(),    buffer._positionList.begin(),   buffer._positionList.end()  );
        _texCoordList.insert(   _texCoordList.end(),    buffer._texCoordList.begin(),   buffer._texCoordList.end()  );
        _normalList.insert(     _normalList.end(),      buffer._normalList.begin(),     buffer._normalList.end()    );
        _tangentList.insert(    _tangentList.end(),     buffer._tangentList.begin(),    buffer._tangentList.end()   );
        _biTangentList.insert(  _biTangentList.end(),   buffer._biTangentList.begin(),  buffer._biTangentList.end() );
        _colorList.insert(      _colorList.end(),       buffer._colorList.begin(),      buffer._colorList.end()     );
    }

    void StreamVertexBuffer::bind() const
    {
        glEnableVertexAttribArray( Position );
        glVertexAttribPointer( Position, 3, GL_FLOAT, GL_FALSE, sizeof( Vector3 ), &_positionList[ 0 ] );

        int vertexProperties = getVertexProperties();

        if( vertexProperties & GG::TEXCOORDS )
        {
            glEnableVertexAttribArray( Uv0 );
            glVertexAttribPointer( Uv0, 2, GL_FLOAT, GL_FALSE, sizeof( Vector2 ), &_texCoordList[ 0 ] );
        }
        else
        {
            glDisableVertexAttribArray( Uv0 );
        }

        if( vertexProperties & GG::NORMALS )
        {
            glEnableVertexAttribArray( Normal );
            glVertexAttribPointer( Normal, 3, GL_FLOAT, GL_FALSE, sizeof( Vector3 ), &_normalList[ 0 ] );
        }
        else
        {
            glDisableVertexAttribArray( Normal );
        }

        if( vertexProperties & GG::TANGENTS )
        {
            glEnableVertexAttribArray( Tangent );
            glVertexAttribPointer( Tangent, 3, GL_FLOAT, GL_FALSE, sizeof( Vector3 ), &_tangentList[ 0 ] );
        }
        else
        {
            glDisableVertexAttribArray( Tangent );
        }

        if( vertexProperties & GG::BITANGENTS )
        {
            glEnableVertexAttribArray( Bitangent );
            glVertexAttribPointer( Bitangent, 3, GL_FLOAT, GL_FALSE, sizeof( Vector3 ), &_biTangentList[ 0 ] );
        }
        else
        {
            glDisableVertexAttribArray( Bitangent );
        }

        if( vertexProperties & GG::COLORS )
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