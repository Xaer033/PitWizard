/*
	Module: Geometry Buffer
	Description: Geometry that we send to the graphics card
	Author: Julian Williams
*/

#pragma once

#include "Vector.h"
#include "IwGL.h"

#include <GG/Core/Types.h>
#include <IVertexBuffer.h>
#include <vector>

namespace GG
{

	class StaticGeometryBuffer : public IVertexBuffer
	{
	public:
		

		struct Vertex
		{
			Vector3		position;
			Vector2		uv0;
			Vector2		uv1;
			Vector3		normal;
			Vector3		tangent;
			Vector3		biTangent;
			Vector4		color;
			Vector4i	boneIndicies;
			Vector4		boneWeights;
		};

	public:
        typedef std::vector<Vertex>     VertexList;
        typedef std::vector<GLuint>     IndexList;


		StaticGeometryBuffer( );
		~StaticGeometryBuffer( );

		virtual void        clearVertices();

		virtual void        addPosition(    const Vector3 & p );
		virtual void        pushUv0(		const Vector2 & t );
		virtual void        pushUv1(		const Vector2 & t );
		virtual void        pushNormal(     const Vector3 & n );
		virtual void        pushColor(	    const Vector4 & t );
		virtual void        pushTangent(    const Vector3 & t );
		virtual void        pushBitangent(  const Vector3 & b );
		virtual void        pushBoneIndices(const Vector4i & i );
		virtual void        pushBoneWeights(const Vector4 & w);

		virtual void		pushIndex(uint index);

		virtual void        build( const DrawHint drawHint = D_STATIC );

        virtual void        render( const DrawMode & drawMode ) const;
		virtual void		render(const DrawMode & drawMode, uint startIndex, uint indexCount) const;

        virtual void        bind() const;


        void                merge( const StaticGeometryBuffer & buffer );

        void                build( Vertex   *   vertexList, uint vertCount,
                                   uint     *   indexList, uint indexCount,
                                   DrawHint     drawHint = D_STATIC );


        const Vertex *      getVertexArray()	const;
        const uint *        getIndexArray()		const;


        uint                getIndexCount()     const;
        uint                getVertexCount()    const;

	private:

        VertexList		    _vertexList;
        IndexList		    _indexList;

        GLuint              _indexBufferHandle;
        GLuint              _vertexBufferHandle;
        GLuint              _arrayBufferHandle;


        Vector2             _currentUv0;
		Vector2             _currentUv1;
        Vector3             _currentNormal;
        Vector4             _currentColor;
        Vector3             _currentTangent;
        Vector3             _currentBitangent;
		Vector4i			_currentBoneIndices;
		Vector4				_currentBoneWeights;


        void                _generateTangents();
        void                _generateIndices();

        bool                _isSameVertex( const Vertex &   v1, const Vertex &  v2 )    const;
	};
}


