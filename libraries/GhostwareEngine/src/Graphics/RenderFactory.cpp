
#include "RenderFactory.h"
#include "Camera.h"
#include "Mesh.h"

#include <IwGx.h>
#include <IwGraphics.h>

#include <GG/Core/Vector.h>
#include <GG/Core/Rect.h>
#include <GG/Core/Log.h>
#include <GG/Core/MathDebug.h>

namespace GG
{
	void RenderFactory::addCommand( Material * mat, Model * geo, const Matrix4 & worldMatrix )
	{
		_renderCommand3DList.push_back( 
			RenderCommand3D { mat, geo, worldMatrix } 
		);
	}

	void RenderFactory::clearAllCommands()
	{
		_renderCommand3DList.clear();
	}

	void RenderFactory::renderAll( const Camera * camera ) const
	{
		if( camera == nullptr )
		{
			LOG_ERROR( "Camera Node does not contain Camera!" );
			return;
		}

		
		_setViewport( camera->getViewport() );
		_clearBuffer( camera->getClearColor(), camera->getClearMode() );

		IwGxSetPerspectiveMatrix( (float*)camera->getProjectionMatrix() );
		IwGxSetViewMatrix( &camera->getViewMatrix() );

		_render3DList();
	}

// -------------------------------------------------------------------------------

	void RenderFactory::_render3DList() const
	{
		Material *	_currentMat		= nullptr;

		for(  
			auto it = _renderCommand3DList.begin(); 
			it != _renderCommand3DList.end(); 
			++it 
		)
		{
			const RenderCommand3D * command = &( *it );
			if( command == nullptr )
			{
				TRACE_ERROR( "Render Command is null!" );
				continue;
			}

			IwGxSetModelMatrix( &command->modelMatrix );

			Material * mat = command->material;
			if( mat != nullptr && mat != _currentMat )
			{
				IwGxSetMaterial( mat );
				_currentMat = mat;
			}

			if( command->geometry != nullptr )
			{ 
				command->geometry->Render( false );
			}
		}
	}

	void RenderFactory::_setViewport( const Vector4 & viewport ) const
	{
		float width		= (float)IwGxGetScreenWidth();
		float height	= (float)IwGxGetScreenHeight();

		Vector2 xVec	=	Vector2( viewport.x, viewport.z ) * width;
		Vector2 yVec	=	Vector2( viewport.y, viewport.w ) * height;

		_IwGxSetViewRect( ( int32 )xVec.x, ( int32 )yVec.x, ( int32 )xVec.y, ( int32 )yVec.y );
	}

	void RenderFactory:: _clearBuffer( 
		const Vector4 & clearColor, 
		uint clearMode ) const
	{
		const float kColorScale = 255.0f;

		if( clearMode != ClearMode::None )
		{
			IwGxClear( ( uint32 )clearMode );
			Vector4 finalColor = clearColor * kColorScale;
			IwGxSetColClear(( uint8 )finalColor.x, ( uint8 )finalColor.y,
							( uint8 )finalColor.z, ( uint8 )finalColor.w );
		}
	}

	void RenderFactory::_opaqueSort( const Vector3 & cameraEye )
	{
		auto opaqueSort = 
			[ &cameraEye ]( RenderCommand3D a, RenderCommand3D b )
		{
			if( a.material == nullptr || b.material == nullptr )
				return false;

			int16 aAlphaMode = ( int16 )a.material->GetAlphaMode();
			int16 bAlphaMode = ( int16 )b.material->GetAlphaMode();

			bool alphaCompare		=	aAlphaMode < bAlphaMode;
			bool userFlagCompare	=	aAlphaMode == bAlphaMode  && 
										a.material->GetUserFlags() < 
										b.material->GetUserFlags();

			return	alphaCompare || userFlagCompare;
		};

		std::sort( 
			_renderCommand3DList.begin(), 
			_renderCommand3DList.end(), 
			opaqueSort 
		);
	}

	void RenderFactory::_transparentSort( const Vector3 & cameraEye )
	{
		auto transparentSort = 
			[ &cameraEye ]( RenderCommand3D a, RenderCommand3D b )
		{
			if( a.material == nullptr || b.material == nullptr )
				return false;

			int16 aAlphaMode = ( int16 )a.material->GetAlphaMode();
			int16 bAlphaMode = ( int16 )b.material->GetAlphaMode();
			
			bool alphaCompare		=	aAlphaMode < bAlphaMode;
			bool userFlagCompare	=	aAlphaMode == bAlphaMode  &&
				a.material->GetUserFlags() <
				b.material->GetUserFlags();

			return	alphaCompare || userFlagCompare;
		};

		std::sort( 
			_renderCommand3DList.begin(), 
			_renderCommand3DList.end(), 
			transparentSort 
		);
	}
}