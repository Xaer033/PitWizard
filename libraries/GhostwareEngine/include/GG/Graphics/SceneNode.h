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

		const nMatrix4 &	modelToWorldMatrix() const;
		const nMatrix4 &	worldToModelMatrix() const;

		const nVector3 		transformPoint( const nVector3 & point );
		const nVector3 		inverseTransformPoint( const nVector3 & point );

		void				detachChildren();

		const SceneNode *	getChild( uint32 index ) const;
		uint32				getChildrenCount() const;

		inline const nVector3 		forward()	const {
			return glm::normalize(nVector3(_modelMatrix * nVector4(0, 0, 1, 0)));
		}
		inline const nVector3 		back()		const {
			return glm::normalize(nVector3(_modelMatrix * nVector4(0, 0, -1, 0)));
		}
		inline const nVector3 		right()		const {
			return glm::normalize(nVector3(_modelMatrix * nVector4(1, 0, 0, 0)));
		}
		inline const nVector3 		left()		const {
			return glm::normalize(nVector3(_modelMatrix * nVector4(-1, 0, 0, 0)));
		}
		inline const nVector3 		up()		const {
			return glm::normalize(nVector3(_modelMatrix * nVector4(0, 1, 0, 0)));
		}
		inline const nVector3 		down()		const {
			return glm::normalize(nVector3(_modelMatrix * nVector4(0, -1, 0, 0)));
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

		nMatrix4			_modelMatrix;
		nMatrix4			_worldMatrix;
		nMatrix4			_inverseMatrix;

		Quaternion			_rotation;

		ChildrenList		_childrenList;

	};
}