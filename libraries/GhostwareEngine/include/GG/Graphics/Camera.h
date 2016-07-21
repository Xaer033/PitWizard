/*
	Module: lights, *CAMERA*, action!
	Description:	Camera object to see the world through and apply
				camera view matrix to world geometry
	Author: Julian Williams
*/

#pragma once

#include <GG/Graphics/Transform.h>
#include <GG/Core/Matrix.h>
#include <GG/Core/Vector.h>
#include <GG/Core/Types.h>
#include <string>

#include "IwGx.h"

namespace GG
{
	enum ClearMode
	{
		None	= 0,
		Color	= IW_GX_COLOUR_BUFFER_F,
		Depth	= IW_GX_DEPTH_BUFFER_F
	};

	class Camera
	{
	public:

		static	bool DepthCompare( const Camera * a, const Camera * b );


		Camera( const std::string & name = "Camera" );
		~Camera();

		inline void				setEnabled( bool enabled );
		inline bool				isEnabled() const;
		
		inline void				setName( const std::string & name );
		inline std::string 		getName( ) const;
		
		inline void				setLayer( int16 layer );
		inline int16			getLayer() const;
		
		inline void				setDepth( int16 depth );
		inline int16			getDepth( ) const;
		
		void					setClearMode( uint clearMode);
		uint					getClearMode( ) const;
		
		void					setClearColor( const Vector4 & clearColor );
		Vector4					getClearColor( ) const;
		
		void					setViewport( float x, float y, float width, float height );
		Vector4					getViewport() const;


		void					setOrthogonal( float left, float right, float bottom, float top, float zNear, float zFar );
		void					setPerspective( float fovy, float aspect, float zNear, float zFar );


		Matrix4	*				getViewMatrix();
		float *					getProjectionMatrix();

	public:
		Transform				transform;

	private:

		bool					_enabled;
		
		int16					_depth;

		int16					_layer;

		uint					_clearMode;

		std::string				_name;

		Matrix4					_viewMat;
		float 					_projectionMat[16];

		Vector4					_cachedClearColor;
		Vector4					_cachedViewport;
	};
}
