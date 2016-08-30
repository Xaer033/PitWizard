// Ghostware Games inc. 2016  -Julian Williams
#pragma once

#include <list>
#include <string>

#include <GG/Core/Vector.h>
#include <GG/Core/Matrix.h>
#include <GG/Core/Quaternion.h>

namespace GG
{
	enum class Space
	{
		WORLD = 0,
		LOCAL
	};

	class MoveableObject;
	class SceneNode;

	typedef std::list< SceneNode * > ChildrenList;


	class SceneNode
	{
	public:
		SceneNode( const std::string & name = "Node" );
		~SceneNode();

		void				setParent( SceneNode * parent );
		SceneNode *			getParent();

		void				attachObject( MoveableObject * renderable );
		MoveableObject	*	getObject();

		void				setPosition( const Vector3 & position );
		const Vector3 		getLocalPosition() const;
		const Vector3 		getWorldPosition() const;

		void				setAxisAngle( float angle, const Vector3 & axis );
		void				setRotation( const Quaternion & rotation );
		const Quaternion 	getRotation() const;

		void				lookAt( const Vector3 & center, const Vector3 & up = -Vector3::g_AxisY );
		void				lookAt( const SceneNode * node, const Vector3 & up = -Vector3::g_AxisY );

		void				translate( const Vector3 & move );
		void				rotate( float angle, const Vector3 & axis );

		const Matrix4 &		modelToWorldMatrix() const;
		const Matrix4 &		worldToModelMatrix() const;

		const Vector3 		transformPoint( const Vector3 & point );
		const Vector3 		inverseTransformPoint( const Vector3 & point );


		inline const Vector3 		forward()	const {
			return _modelMatrix.RowZ();
		}
		inline const Vector3 		back()		const {
			return -_modelMatrix.RowZ();
		}
		inline const Vector3 		right()		const {
			return _modelMatrix.RowX();
		}
		inline const Vector3 		left()		const {
			return -_modelMatrix.RowX();
		}
		inline const Vector3 		up()		const {
			return _modelMatrix.RowY();
		}
		inline const Vector3 		down()		const {
			return -_modelMatrix.RowY();
		}

	private:

		void		_updateHierarchy( bool updateFromParent = true );
		void		_updateChildren();

		void		_addChild( SceneNode * child );
		void		_removeChild( SceneNode * child );

	private:
		std::string			_name;
		uint32				_id;

		SceneNode *			_parent;
		MoveableObject *	_moveableObject;

		Matrix4			_modelMatrix;
		Matrix4			_worldMatrix;
		Matrix4			_inverseMatrix;

		Quaternion		_rotation;

		ChildrenList	_childrenList;

	};
}