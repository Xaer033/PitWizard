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

	class RenderableObject;
	class SceneNode;

	typedef std::list< SceneNode * > ChildrenList;


	class SceneNode
	{
	public:
		SceneNode();
		~SceneNode();

		void				setParent( SceneNode * parent );
		SceneNode *			getParent();

		void				attachObject( RenderableObject * renderable );
		RenderableObject *	getObject();

		void				setPosition( const nVector3 & position );
		const nVector3 		getLocalPosition() const;
		const nVector3 		getWorldPosition() const;

		void				setAxisAngle( float angle, const nVector3 & axis );
		void				setRotation( const Quaternion & rotation );
		const Quaternion 	getRotation() const;

		void				lookAt( const nVector3 & center, const nVector3 & up = Vector::up());
		void				lookAt( const SceneNode * node, const nVector3 & up = Vector::up());

		void				translate( const nVector3 & move );
		void				rotate( float angle, const nVector3 & axis );

		const Matrix4 &		modelToWorldMatrix() const;
		const Matrix4 &		worldToModelMatrix() const;

		const nVector3 		transformPoint( const nVector3 & point );
		const nVector3 		inverseTransformPoint( const nVector3 & point );

		void				detachChildren();

		const SceneNode *	getChild( uint32 index ) const;
		uint32				getChildrenCount() const;

		inline const nVector3 		forward()	const {
			Vector3 v = _modelMatrix.RowZ();
			return nVector3(v.x, v.y, v.z);
		}
		inline const nVector3 		back()		const {
			Vector3 v = -_modelMatrix.RowZ();
			return nVector3(v.x, v.y, v.z);
		}
		inline const nVector3 		right()		const {
			Vector3 v = _modelMatrix.RowX();
			return nVector3(v.x, v.y, v.z);
		}
		inline const nVector3 		left()		const {
			Vector3 v = -_modelMatrix.RowX();
			return nVector3(v.x, v.y, v.z);
		}
		inline const nVector3 		up()		const {
			Vector3 v = _modelMatrix.RowY();
			return nVector3(v.x, v.y, v.z);
		}
		inline const nVector3 		down()		const {
			Vector3 v = -_modelMatrix.RowY();
			return nVector3(v.x, v.y, v.z);
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
		RenderableObject *	_renderableObject;

		Matrix4			_modelMatrix;
		Matrix4			_worldMatrix;
		Matrix4			_inverseMatrix;

		Quaternion		_rotation;

		ChildrenList	_childrenList;

	};
}