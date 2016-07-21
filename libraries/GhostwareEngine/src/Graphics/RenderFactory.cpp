
#include "RenderFactory.h"
#include "Camera.h"

#include <IwGx.h>
#include <IwGraphics.h>

#include <GG/Core/Vector.h>
#include <GG/Core/Rect.h>

namespace GG
{

	void RenderFactory::addCommand( Material * mat, CIwModel * geo, Matrix4 * worldMatrix )
	{
		_renderCommand3DList.push_back( RenderCommand3D { mat, geo, worldMatrix } );
	}

	void RenderFactory::clearAllCommands()
	{
		_renderCommand3DList.clear();
	}

	void RenderFactory::renderAll( Camera * camera )
	{
		if( camera == nullptr )
		{
			return;
		}

		_setViewport( camera->getViewport() );
		_clearBuffer( camera->getClearColor(), camera->getClearMode() );

		IwGxSetPerspectiveMatrix( camera->getProjectionMatrix() );
		IwGxSetViewMatrix( camera->getViewMatrix() );
		

		auto it = _renderCommand3DList.begin();
		while(it != _renderCommand3DList.end() )
		{
			RenderCommand3D * command = &( *it );
			if( command != nullptr )
			{
				if( command->material != nullptr )
					IwGxSetMaterial( command->material );

				if( command->modelMatrix !=  nullptr )
					IwGxSetModelMatrix( command->modelMatrix );

				if( command->geometry != nullptr )
					command->geometry->Render( false );

				++it;
			}
			else
			{
				_renderCommand3DList.erase( it );
			}
		}
	}


	void RenderFactory::_setViewport( const Vector4 & viewport )
	{
		float width		= (float)IwGxGetScreenWidth();
		float height	= (float)IwGxGetScreenHeight();

		Vector2 xVec	=	Vector2( viewport.x, viewport.z ) * width;
		Vector2 yVec	=	Vector2( viewport.y, viewport.w ) * height;
		//IwGxSetScissorScreenSpace(()
		_IwGxSetViewRect( ( int32 )xVec.x, ( int32 )yVec.x, ( int32 )xVec.y, ( int32 )yVec.y );
	}

	void RenderFactory:: _clearBuffer( const Vector4 & clearColor, uint clearMode )
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
		auto opaqueSort = [ &cameraEye ]( RenderCommand3D a, RenderCommand3D b )
		{
			if( a.material == nullptr || b.material == nullptr )
				return false;

			int16 aAlphaMode = ( int16 )a.material->GetAlphaMode();
			int16 bAlphaMode = ( int16 )b.material->GetAlphaMode();

			return	( aAlphaMode < bAlphaMode ) ||
					( aAlphaMode == bAlphaMode  && a.material->GetUserFlags() < b.material->GetUserFlags() );
		};

		std::sort( _renderCommand3DList.begin(), _renderCommand3DList.end(), opaqueSort );
	}

	void RenderFactory::_transparentSort( const Vector3 & cameraEye )
	{
		auto transparentSort = [ &cameraEye ]( RenderCommand3D a, RenderCommand3D b )
		{
			if( a.material == nullptr || b.material == nullptr )
				return false;

			int16 aAlphaMode = ( int16 )a.material->GetAlphaMode();
			int16 bAlphaMode = ( int16 )b.material->GetAlphaMode();

			return	( aAlphaMode < bAlphaMode ) ||
					( aAlphaMode == bAlphaMode  && a.material->GetUserFlags() < b.material->GetUserFlags() );
		};

		std::sort( _renderCommand3DList.begin(), _renderCommand3DList.end(), transparentSort );
	}
}