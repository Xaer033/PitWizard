

#include "SceneNode.h"
#include "RenderableObject.h"

#include <GG/Core/Types.h>
#include <GG/Core/Vector.h>
#include <GG/Core/Quaternion.h>

#include "s3eDebug.h"
#include "IwHashString.h"
#include <cmath>
#include <string>


namespace GG
{
	SceneNode::SceneNode( ) :
		_parent( nullptr ),
		_renderableObject( nullptr ),
		_modelMatrix( Matrix4::g_Identity ),
		_worldMatrix( Matrix4::g_Identity ),
		_inverseMatrix( Matrix4::g_Identity)
	{
		Vector3 zero = Vector3(0, 0, 0);
		_modelMatrix.t		= zero;
		_worldMatrix.t		= zero;
		_inverseMatrix.t	= zero;
	}

	SceneNode::~SceneNode()
	{
		_parent = nullptr;
		_childrenList.clear();
	}


	void SceneNode::setParent( SceneNode * parent )
	{
		if( _parent == parent )
			return;
		
		SceneNode * old = _parent;
		_parent = parent;

		if( _parent != nullptr )
			_parent->_addChild( this );

		if( old != nullptr )
			old->_removeChild( this );

		_updateHierarchy();
	}

	SceneNode * SceneNode::getParent()
	{
		return _parent;
	}

	void SceneNode::attachObject( RenderableObject * moveableObject )
	{
		_renderableObject = moveableObject;
	}

	RenderableObject * SceneNode::getObject()
	{
		return _renderableObject;
	}

	void SceneNode::setPosition( const nVector3 & position )
	{
		_modelMatrix.t = Vector3(position.x, position.y, position.z);
		_updateHierarchy( false );
	}

	const nVector3 SceneNode::getLocalPosition() const
	{
		Vector3 pos = _modelMatrix.t;
		return nVector3(pos.x, pos.y, pos.z);
	}

	const nVector3 SceneNode::getWorldPosition() const
	{
		Vector3 pos = _worldMatrix.t;
		return nVector3(pos.x, pos.y, pos.z);
	}

	void SceneNode::setAxisAngle( float angle, const nVector3 & axis )
	{
		_rotation = glm::quat(angle, axis);

		nVector3 pos = getLocalPosition();
		_modelMatrix.SetAxisAngle( Vector3(axis.x, axis.y, axis.z), angle );
		_modelMatrix.t = Vector3(pos.x, pos.y, pos.z);

		_updateHierarchy();
	}

	void SceneNode::setRotation( const Quaternion & rotation )
	{
		_rotation = rotation;
		nVector3 axis = glm::axis(rotation);

		nVector3 pos = getLocalPosition();
		_modelMatrix.SetAxisAngle( Vector3(axis.x, axis.y, axis.z), glm::angle(rotation));
		_modelMatrix.t = Vector3(pos.x, pos.y, pos.z);

		_updateHierarchy();
	}

	const Quaternion SceneNode::getRotation() const
	{
		return _rotation;
	}


	void SceneNode::lookAt( const nVector3 & center, const nVector3 & up )
	{
		nVector3 pos	= getWorldPosition();
		nVector3 delta	= pos - center;

		if( ( fabs( delta.x ) < 0.001f ) &&
			( fabs( delta.y ) < 0.001f ) &&
			( fabs( delta.z ) < 0.001f ) )
			return;

		_modelMatrix.LookAt( 
			Vector3(pos.x, pos.y, pos.z),
			Vector3(center.x, center.y, center.z),
			Vector3(up.x, up.y, up.z)
		);

		_updateHierarchy();
	}

	void SceneNode::lookAt( const SceneNode * center, const nVector3 & up )
	{
		lookAt( center->getWorldPosition(), up );
	}

	void SceneNode::translate( const nVector3 & move )
	{
		_modelMatrix.t += Vector3(move.x, move.y, move.z);
		_updateHierarchy();
	}

	void SceneNode::rotate( float angle, const nVector3 & axis )
	{
		_rotation = glm::quat(angle, axis);

		Matrix4 newRot;
		newRot.SetAxisAngle( Vector3(axis.x, axis.y, axis.z), angle );
		_modelMatrix = newRot * _modelMatrix;

		_updateHierarchy();
	}

	const Matrix4 & SceneNode::modelToWorldMatrix() const
	{
		return _worldMatrix;
	}

	const Matrix4 & SceneNode::worldToModelMatrix() const
	{
		return _inverseMatrix;
	}

	const nVector3 SceneNode::transformPoint( const nVector3 & point )
	{
		Vector3 p(point.x, point.y, point.z);
		Vector3 t = _worldMatrix.TransformVec(p);
		return nVector3(t.x, t.y, t.z);
	}

	const nVector3 SceneNode::inverseTransformPoint( const nVector3 & point )
	{
		Vector3 p(point.x, point.y, point.z);
		Vector3 t = _worldMatrix.TransposeTransformVec(p);
		return nVector3(t.x, t.y, t.z);
	}


	void SceneNode::detachChildren()
	{
		auto it = _childrenList.rbegin();
		for(; it != _childrenList.rend(); --it )
		{
			_removeChild( (*it) );
		}
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

		CIwFVec3 invPos		= _worldMatrix.t;
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