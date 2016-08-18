

#include "SceneNode.h"

#include <GG/Core/Types.h>
#include <GG/Core/Vector.h>
#include <GG/Core/Quaternion.h>

#include "s3eDebug.h"
#include <cmath>


namespace GG
{
	SceneNode::SceneNode()
	{
		_parent				= nullptr;
		_modelMatrix		= Matrix4::g_Identity;
		_modelMatrix.t		= Vector3::g_Zero;
		_worldMatrix		= Matrix4::g_Identity;
		_worldMatrix.t		= Vector3::g_Zero;
		_inverseMatrix		= Matrix4::g_Identity;
		_inverseMatrix.t	= Vector3::g_Zero;
	}

	SceneNode::~SceneNode()
	{
		_parent = nullptr;
		_childrenList.clear();
	}


	void SceneNode::setParent( SceneNode * parent )
	{
		if( _parent == parent )
		{
			return;
		}

		
		SceneNode * old = _parent;
		_parent = parent;

		if( _parent != nullptr )
		{
			_parent->_addChild( this );
		}

		if( old != nullptr )
		{
			old->_removeChild( this );
		}

		_updateHierarchy();
	}

	SceneNode * SceneNode::getParent()
	{
		return _parent;
	}
	void SceneNode::setPosition( const Vector3 & position )
	{
		_modelMatrix.t = position;

		_updateHierarchy( false );
	}

	const Vector3 SceneNode::getLocalPosition() const
	{
		return _modelMatrix.t;
	}

	const Vector3 SceneNode::getWorldPosition() const
	{
		return _worldMatrix.t;
	}

	void SceneNode::setAxisAngle( float angle, const Vector3 & axis )
	{
		_rotation.SetAxisAngle( axis.x, axis.y, axis.z, angle );

		Vector3 pos = getLocalPosition();
		_modelMatrix.SetAxisAngle( axis, angle );
		_modelMatrix.t = pos;

		_updateHierarchy();
	}

	void SceneNode::setRotation( const Quaternion & rotation )
	{
		_rotation = rotation;

		Vector3 pos = getLocalPosition();
		_modelMatrix.SetAxisAngle( _rotation.GetRotationAxis(), _rotation.GetRotationAngle() );
		_modelMatrix.t = pos;

		_updateHierarchy();
	}

	const Quaternion SceneNode::getRotation() const
	{
		return _rotation;
	}


	void SceneNode::lookAt( const Vector3 & center, const Vector3 & up )
	{
		Vector3 pos		= getWorldPosition();
		Vector3 delta	= pos - center;

		if( ( fabs( delta.x ) < 0.001f ) &&
			( fabs( delta.y ) < 0.001f ) &&
			( fabs( delta.z ) < 0.001f ) )
			return;

		_modelMatrix.LookAt( pos, center, up );

		_updateHierarchy();
	}

	void SceneNode::lookAt( const SceneNode * center, const Vector3 & up )
	{
		lookAt( center->getWorldPosition(), up );
	}

	void SceneNode::translate( const Vector3 & move )
	{
		_modelMatrix.t += move;
		_updateHierarchy();
	}

	void SceneNode::rotate( float angle, const Vector3 & axis )
	{
		_rotation.SetAxisAngle( axis.x, axis.y, axis.z, angle );

		Matrix4 newRot;
		newRot.SetAxisAngle( axis, angle );
		_modelMatrix = newRot * _modelMatrix;

		_updateHierarchy();
		//setRotation( _rotation * newRot );
	}

	const Matrix4 & SceneNode::modelToWorldMatrix() const
	{
		return _worldMatrix;
	}

	const Matrix4 & SceneNode::worldToModelMatrix() const
	{
		return _inverseMatrix;
	}

	const Vector3 SceneNode::transformPoint( const Vector3 & point )
	{
		return _worldMatrix.TransformVec( point );
	}

	const Vector3 SceneNode::inverseTransformPoint( const Vector3 & point )
	{
		return _worldMatrix.TransposeTransformVec( point );
	}

// ------------------------- Private Methods ----------------------------------

	void SceneNode::_updateHierarchy( bool updateFromParent )
	{
		if( _parent == nullptr || !updateFromParent )
		{
			_worldMatrix = _modelMatrix;
		}
		else
		{
			_worldMatrix = _modelMatrix *  _parent->_worldMatrix;
		}

		Vector3 invPos		= _worldMatrix.t;
		_inverseMatrix		= _worldMatrix.GetTranspose();
		_inverseMatrix.t	= _inverseMatrix.TransformVec( invPos );

		_updateChildren();
	}


	void SceneNode::_updateChildren()
	{
		//Loop threw children to update them
		auto it = _childrenList.begin();

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

	void SceneNode::_addChild( SceneNode * child )
	{

		Vector3 invPos			= child->_modelMatrix.t;
		child->_modelMatrix		= child->_modelMatrix * worldToModelMatrix();
		child->_modelMatrix.t	= modelToWorldMatrix().TransposeTransformVec( invPos );

		_childrenList.push_back( child );
	}

	void SceneNode::_removeChild( SceneNode * child )
	{
		Vector3 invPos			= child->_modelMatrix.t;
		child->_modelMatrix		= child->_modelMatrix * modelToWorldMatrix();
		child->_modelMatrix.t	= modelToWorldMatrix().TransformVec( invPos );

		_childrenList.remove( child );
	}
}