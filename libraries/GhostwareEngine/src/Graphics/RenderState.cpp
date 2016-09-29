
#include "RenderState.h"

#include <vector>
#include <unordered_map>


#include <IwGL.h>
#include <IwGLExt.h>

#include <GG/Graphics/Shader.h>
#include <GG/Graphics/Texture.h>
//#include <FrameBufferObject.h>


//#define BUFFER_OFFSET(i) ((void*)(i))


namespace GG
{
	RenderState::RenderState()
	{
		_clearCache();
	}

	RenderState::~RenderState()
	{
		_clearCache();
	}

	void RenderState::clearRenderBuffer( int mode ) const
	{
		glClear( mode );
	}

	void RenderState::setClearColor( const Vector4 & clearColor ) const
	{
		glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
	}

	void RenderState::setDepthTesting( bool depthTest )
	{
		if( _cachedDepthTest == depthTest )
			return;

		if( depthTest )
			glEnable( GL_DEPTH_TEST );
		else
			glDisable( GL_DEPTH_TEST );

		_cachedDepthTest = depthTest;
	}

	void RenderState::setDepthRange( float zNear, float zFar )
	{
		glDepthRangef( zNear, zFar );
	}

	void RenderState::setCullMode( const CullMode cullMode )
	{
		if( _cachedCullMode == cullMode )
			return;

        if( cullMode == CULL_NONE )
		{
			glDisable( GL_CULL_FACE );
		}
		else
		{
			glEnable( GL_CULL_FACE );
			glCullFace( (GLenum)cullMode );
		}

		_cachedCullMode = cullMode;

	}

	void RenderState::setBlendmode( const BlendMode mode )
	{
		if( _cachedBlendmode == mode )
			return;

		_cachedBlendmode = mode;


        if( mode == BM_NONE )
		{
			glDisable( GL_BLEND );
			return;
		}
				

		glEnable( GL_BLEND );

		switch( mode )
		{
            case BM_ALPHA:		glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );	break;
			case BM_ADDITIVE:	glBlendFunc( GL_SRC_ALPHA, GL_ONE );					break;
            case BM_MULTIPLY:	glBlendFunc( GL_DST_COLOR, GL_ONE_MINUS_SRC_ALPHA );	break;
            case BM_SCREEN:		glBlendFunc( GL_ONE, GL_ONE_MINUS_SRC_COLOR );			break;
		}
	}


	void RenderState::setRenderSize( int width, int height )
	{
		_renderWidth	= width;
		_renderHeight	= height;
	}

	void RenderState::setViewport(const Vector4 & viewport) const
	{
		Vector2 xVec = Vector2(viewport.x, viewport.z) * (float)_renderWidth;
		Vector2 yVec = Vector2(viewport.y, viewport.w) * (float)_renderHeight;

		glViewport((GLint)xVec.x, (GLint)yVec.x, (GLsizei)xVec.y, (GLsizei)yVec.y );
	}



	void RenderState::bindTexture2d( uint samplerID, const Texture & texture )
	{
		TextureCache::iterator it =  _cachedTextures.find( samplerID );

		uint32 texHandle = texture.m_HWID;

		if( it == _cachedTextures.end() )
			_setTexture( samplerID, texHandle );
		else if( it->second != texHandle )
			_setTexture( samplerID, texHandle );
	}


	void RenderState::bindShader( const Shader & shader )
	{
		uint shaderId = shader.getId();

		if( shaderId == _cachedShader )
			return;

		glUseProgram( shaderId );

		_cachedShader = shaderId;
	}


	//void RenderState::bindFrameBuffer( const uint & frameBuffer )
	//{
	//	glBindFramebuffer( GL_FRAMEBUFFER, frameBuffer );
	//}

	void RenderState::_clearCache()
	{
		_cachedShader		= 0;

		_cachedFrameBuffer	= 0;

		_cachedCullMode		= CULL_BACK;

		_renderWidth		= IwGLGetInt( IW_GL_WIDTH );
		_renderHeight		= IwGLGetInt( IW_GL_HEIGHT );

		_cachedTextures.clear();
	}

	void RenderState::_setTexture( uint samplerId, uint texHandle )
	{
		glActiveTexture( GL_TEXTURE0 + samplerId );
		glBindTexture( GL_TEXTURE_2D, texHandle );

		_cachedTextures[ samplerId ] = texHandle;
	}
}
