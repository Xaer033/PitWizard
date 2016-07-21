/*
	Module: Transform
	Description:	Transform for game objects, essentially their worldMatrix
	Author: Julian Williams
*/

#pragma once

#include <list>

#include <GG/Core/Types.h>
#include <GG/Core/Vector.h>
#include <GG/Core/Matrix.h>



namespace GG
{

	enum class Space
	{
		WORLD = 0,
		LOCAL
	};


	class Transform;

	typedef std::list< Transform * > ChildrenList;


	class Transform
	{
	public:

		Transform();
		virtual ~Transform( );


		inline Vector3	forward()	const;
		inline Vector3	back()		const;
		inline Vector3	right()		const;
		inline Vector3	left()		const;
		inline Vector3	up()		const;
		inline Vector3	down()		const;
	
	
	
		inline Matrix4	getMatrix() const;

		void		    setParent( Transform * parent );
		Transform *     getParent();

		void	        addChild( Transform * child );
		void	        removeChild( Transform * child );


		void	        lookAt( const Vector3 & center, const Vector3 & up = Vector3( 0, 1, 0 ) );
		void	        lookAt( const Transform & center, const Vector3 & up = Vector3( 0, 1, 0 )  );

		void	        setPosition( const Vector3 & position );
		Vector3         getPosition( Space space = Space::LOCAL ) const;

		void	        setAxisAngle( float angle, const Vector3 & axis );
		void	        setScale(const Vector3 & scale );
	
		void	        translate( const Vector3 & translation, Space moveSpace = Space::LOCAL );
		void	        rotate( float angle, const Vector3 & axis );
		void	        scale( const Vector3 & scale, Space scaleSpace = Space::LOCAL );


		inline Matrix4 	getInverse() const;

        inline Vector3	transformPoint( const Vector3 & point );
        inline Vector3	inverseTransformPoint( const Vector3 & point );

	
	private:

		void	        _updateHierarchy( );
		void	        _updateChildren( );

	private:
		
		Matrix4			_modelMat;
		Matrix4			_worldMat;
        Matrix4			_inverseMat;

		Transform *     _parent;

		ChildrenList    _childrenList;
	
	};
}

