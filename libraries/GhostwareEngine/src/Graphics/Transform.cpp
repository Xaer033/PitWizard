
#include "Transform.h"

#include <list>	

#include <GG/Core/Matrix.h>
#include <GG/Core/Vector.h>


namespace GG
{

	Transform::Transform()
	{
		_parent = NULL;
		_modelMat = Matrix4::g_Identity;
		_updateHierarchy();
	}

	Transform::~Transform( )
	{
	}
	

	inline Vector3 Transform::forward()	const
	{
		return -back();
	}

	inline Vector3 Transform::back() const
	{
		return Vector3( _worldMat.m[ 2 ][ 0 ], _worldMat.m[ 2 ][ 1 ], _worldMat.m[ 2 ][ 2 ] ).GetNormalised();
	}
	
	inline Vector3 Transform::right() const
	{
		Vector3( _worldMat.m[ 0 ][ 0 ], _worldMat.m[ 0 ][ 1 ], _worldMat.m[ 0 ][ 2 ] ).GetNormalised();
	}
	
	inline Vector3 Transform::left() const
	{
		return -right();
	}

	inline Vector3 Transform::up() const
	{
		Vector3( _worldMat.m[ 1 ][ 0 ], _worldMat.m[ 1 ][ 1 ], _worldMat.m[ 1 ][ 2 ] ).GetNormalised();
	}

	inline Vector3 Transform::down() const
	{
		return -up();
	}


	inline Matrix4 Transform::getMatrix() const
	{
		return _worldMat;
	}

	void Transform::setParent( Transform * parent )
	{
		/*if( _parent == parent )
			return; 

		Transform * oldp = _parent;
		_parent = parent;

		if( _parent != NULL )
		{
			glm::mat4 invParent = _parent->getMatrix();
			invParent = glm::inverse( invParent );

			_modelMat =  invParent * _worldMat;
			
			_parent->addChild( this );


		}else{

			if( oldp != NULL )
				oldp->removeChild( this );

			_modelMat = _worldMat;
		}*/

		_updateHierarchy( );
	}

	Transform * Transform::getParent()
	{
		return _parent;
	}


	void Transform::addChild( Transform * child )
	{
		_childrenList.push_back( child );
	}

	void Transform::removeChild( Transform * child )
	{
		_childrenList.remove( child );
	}


	void Transform::lookAt( const Vector3 & center, const Vector3 & up )
	{
		Vector3 pos		= getPosition();
		Vector3 delta	= pos - center;

		if(	( fabs( delta.x ) < 0.001f) &&
			( fabs( delta.y ) < 0.001f) &&
			( fabs( delta.z ) < 0.001f)) 
			return;

		_modelMat.LookAt( pos, center, up );

		_updateHierarchy( );
	}

	void Transform::lookAt( const Transform & center, const Vector3 & up )
	{
		lookAt( center.getPosition(), up );
	}

	Vector3 Transform::getPosition( Space space ) const
	{
		if( space == Space::LOCAL )
			return _modelMat.t;

		return _worldMat.t;
	}

	void Transform::setPosition( const Vector3 & position )
	{
		_modelMat.t = position;
		_updateHierarchy( );
	}

	void Transform::setAxisAngle( float angle, const Vector3 & axis )
	{
     
		_modelMat.SetAxisAngle( axis, angle );

		_updateHierarchy( );
	}


	void Transform::setScale( const Vector3 & scale )
	{
		/*_modelMat[0][3]	= scale.x;
		_modelMat[1][3]	= scale.y;
		_modelMat[2][3]	= scale.z;*/

		_updateHierarchy( );
	}

	void Transform::translate( const Vector3 & translation, Space moveSpace )
	{
		if( moveSpace == Space::LOCAL && _parent != nullptr )
		{
			_modelMat.t =  _modelMat.TransformVec( translation );
		}
		else
		{
			_modelMat.t +=  _modelMat.TransformVec( translation );
		}

		_updateHierarchy( );
	}

	void Transform::rotate( float angle, const Vector3 & axis )
	{
		Matrix4 newRot = Matrix4::g_Identity;
		newRot.SetAxisAngle( axis, angle );

		_modelMat = _modelMat * newRot;
		
		_updateHierarchy( );
	}

	void Transform::scale( const Vector3 & scale, Space scaleSpace )
	{
		//_modelMat = glm::scale( _modelMat, scale );

		_updateHierarchy( );
	}

	inline Matrix4 Transform::getInverse() const
	{
        return _inverseMat;
	}


    inline Vector3 Transform::transformPoint( const Vector3 & point )
    {
		return _worldMat.TransformVec( point );
    }

    inline Vector3 Transform::inverseTransformPoint( const Vector3 & point )
    {
		return _inverseMat.TransformVec( point );
    }


//--------------------------------- Private Implementation -------------------------------

	void Transform::_updateHierarchy( )
	{
		if( _parent != NULL )
			_worldMat =  _parent->getMatrix() * _modelMat;
		else
			_worldMat = _modelMat;

		_inverseMat =	_worldMat.GetTranspose();
		_inverseMat.t = _worldMat.TransposeTransformVec( _worldMat.t );

		_updateChildren();
	}

	void Transform::_updateChildren()
	{
		//Loop threw children to update them
        std::list<Transform*>::iterator it = _childrenList.begin();

        while( it != _childrenList.end() )
        {
            if( *it )
            {
                ( *it )->_updateHierarchy();
                ++it;
            }
            else
            {
                it = _childrenList.erase( it );
            }
		}
	}
}
