

#include "SceneNode.h"
#include "RenderableObject.h"

#include <GG/Core/Types.h>
#include <GG/Core/Vector.h>
#include <GG/Core/Quaternion.h>
#include <GG/Core/Log.h>

#include <cmath>
#include <string>


namespace GG
{
	SceneNode::SceneNode( ) :
		_parent( nullptr ),
		_renderableObject( nullptr ),
		_modelMatrix(Matrix::identity()),
		_worldMatrix(Matrix::identity()),
		_inverseMatrix(Matrix::identity())
	{
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

	void SceneNode::setPosition( const Vector3 & position )
	{
		_modelMatrix[3][0] = position.x;
		_modelMatrix[3][1] = position.y;
		_modelMatrix[3][2] = position.z;
		
		_updateHierarchy( false );
	}

	const Vector3 SceneNode::getLocalPosition() const
	{
		return Vector3(
			_modelMatrix[3][0], 
			_modelMatrix[3][1], 
			_modelMatrix[3][2]);
	}

	const Vector3 SceneNode::getWorldPosition() const
	{
		return Vector3(
			_worldMatrix[3][0],
			_worldMatrix[3][1],
			_worldMatrix[3][2]);
	}

	void SceneNode::setAxisAngle( const Angle & angle, const Vector3 & axis )
	{
		setRotation(Quaternion(angle.toDegrees(), axis));
	}

	void SceneNode::setRotation( const Quaternion & rotation )
	{
		_rotation = rotation;

		Vector3 pos		= getLocalPosition();
		_modelMatrix	= glm::toMat4(_rotation);

		_modelMatrix[3][0] = pos.x;
		_modelMatrix[3][1] = pos.y;
		_modelMatrix[3][2] = pos.z;

		_updateHierarchy();
	}

	const Quaternion SceneNode::getRotation() const
	{
		return _rotation;
	}


	void SceneNode::lookAt( const Vector3 & center, const Vector3 & up )
	{
		Vector3 pos	= getWorldPosition();
		Vector3 delta	= pos - center;

		if( ( fabs( delta.x ) < 0.001f ) &&
			( fabs( delta.y ) < 0.001f ) &&
			( fabs( delta.z ) < 0.001f ) )
			return;

		_modelMatrix = glm::lookAt(pos, center, up);
		_modelMatrix = glm::inverse(_modelMatrix);

		_updateHierarchy();
	}

	void SceneNode::lookAt( const SceneNode * center, const Vector3 & up )
	{
		lookAt( center->getWorldPosition(), up );
	}

	void SceneNode::translate( const Vector3 & move, const Space & space)
	{
		if(space == Space::WORLD)
		{
			_modelMatrix[3][0] += move.x;
			_modelMatrix[3][1] += move.y;
			_modelMatrix[3][2] += move.z;

			_updateHierarchy(false);
		}
		else
		{
			_modelMatrix = glm::translate(_modelMatrix, move);
			_updateHierarchy();
		}
	}

	void SceneNode::rotate( const Angle & angle, const Vector3 & axis )
	{
		//setRotation(glm::rotate(_rotation, angle.toRadians(), axis));
		_modelMatrix = glm::rotate(_modelMatrix, angle.toRadians(), axis);
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

	const Vector3 SceneNode::transformPoint( const Vector3 & point )
	{
		return Vector3(_worldMatrix * Vector4(point, 1));
	}

	const Vector3 SceneNode::inverseTransformPoint( const Vector3 & point )
	{
		return Vector3(_inverseMatrix * Vector4(point, 1));
	}


	void SceneNode::detachChildren()
	{
		auto it = _childrenList.rbegin();
		for(; it != _childrenList.rend(); --it )
		{
			_removeChild( (*it) );
		}
	}

	const SceneNode * SceneNode::getChild(uint32 index) const
	{
		if(index < 0 || index >= _childrenList.size())
		{
			LOG_ERROR("Index is out of bounds!");
			return nullptr;
		}
		return (*std::next(_childrenList.begin(), index));
	}

	uint32	SceneNode::getChildrenCount() const
	{
		return _childrenList.size();
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
			_worldMatrix = _parent->_worldMatrix * _modelMatrix;
		}

		_inverseMatrix = glm::inverse(_worldMatrix);
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
		child->_modelMatrix		= _inverseMatrix * child->_modelMatrix;
		_childrenList.push_back( child );
	}

	void SceneNode::_removeChild( SceneNode * child )
	{
		child->_modelMatrix		= _worldMatrix * child->_modelMatrix;
		_childrenList.remove( child );
	}
}
