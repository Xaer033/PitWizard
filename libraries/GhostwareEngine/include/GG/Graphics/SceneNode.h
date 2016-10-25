// Ghostware Games inc. 2016  -Julian Williams
#pragma once

#include <list>
#include <string>

#include <GG/Core/Vector.h>
#include <GG/Core/Matrix.h>
#include <GG/Core/Quaternion.h>
#include <GG/Core/Angle.h>

namespace GG
{
	class IRenderableObject;
	class SceneNode;

	typedef std::list< SceneNode * > ChildrenList;

	enum class Space
	{
		WORLD,
		LOCAL
	};

	class SceneNode
	{
	public:
		SceneNode();
		~SceneNode();

		void				setParent( SceneNode * parent );
		SceneNode *			getParent();

		void				attachObject( IRenderableObject * renderable );
		IRenderableObject *	getObject();

		void				setPosition( const Vector3 & position );
		const Vector3 		getLocalPosition() const;
		const Vector3 		getWorldPosition() const;

		void				setAxisAngle( const Angle & angle, const Vector3 & axis );
		void				setRotation( const Quaternion & rotation );
		const Quaternion 	getRotation() const;

		void				lookAt( const Vector3 & center, const Vector3 & up = Vector::up());
		void				lookAt( const SceneNode * node, const Vector3 & up = Vector::up());

		void				translate( const Vector3 & move, const Space & space = Space::WORLD );
		void				rotate( const Angle & angle, const Vector3 & axis );

		const Matrix4 &		modelToWorldMatrix() const;
		const Matrix4 &		worldToModelMatrix() const;

		const Vector3 		transformPoint( const Vector3 & point );
		const Vector3 		inverseTransformPoint( const Vector3 & point );

		void				detachChildren();

		const SceneNode *	getChild( uint32 index ) const;
		uint32				getChildrenCount() const;


		inline const Vector3 		forward()	const {
			return glm::normalize(Vector3(_modelMatrix * Vector4( 0, 0, -1, 0)));
		}
		inline const Vector3 		back()		const {
			return -forward();
		}
		inline const Vector3 		right()		const {
			return glm::normalize(Vector3(_modelMatrix * Vector4(1, 0, 0, 0)));
		}
		inline const Vector3 		left()		const {
			return -left();
		}
		inline const Vector3 		up()		const {
			return glm::normalize(Vector3(_modelMatrix * Vector4(0, 1, 0, 0)));
		}
		inline const Vector3 		down()		const {
			return -down();
		}

	private:

		void				_updateHierarchy( bool updateFromParent = true );
		void				_updateChildren();

		void				_addChild( SceneNode * child );
		void				_removeChild( SceneNode * child );

	private:
		std::string			_name;
		uint32				_id;

		SceneNode *			_parent;
		IRenderableObject *	_renderableObject;

		Matrix4				_modelMatrix;
		Matrix4				_worldMatrix;
		Matrix4				_inverseMatrix;

		Quaternion			_rotation;

		ChildrenList		_childrenList;

	};
}