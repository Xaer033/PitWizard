// Ghostware Games inc. 2016  -Julian Williams
#pragma once

#include <unordered_map>

#include "IwGL.h"

#include <GG/Core/Types.h>
#include <GG/Core/Vector.h>
#include <GG/Graphics/Texture2D.h>
#include <GG/Graphics/Shader.h>

namespace GG
{


	class FrameBufferObject;

	class RenderState
	{
	public:
		enum BlendMode
		{
			BM_NONE     = 0,
			BM_ALPHA,
			BM_ADDITIVE,
			BM_MULTIPLY,
			BM_SCREEN
		};

		enum ClearMode
		{
			CM_NONE		= 0,
			CM_COLOR	= GL_COLOR_BUFFER_BIT,
			CM_DEPTH	= GL_DEPTH_BUFFER_BIT,
			CM_STENCIL	= GL_STENCIL_BUFFER_BIT
		};

		enum CullMode
		{
			CULL_NONE   = 0,
			CULL_FRONT	= GL_FRONT,
			CULL_BACK	= GL_BACK,
			CULL_FRONT_AND_BACK = GL_FRONT_AND_BACK
		};

		enum WindingMode
		{
			WM_CLOCKWISE = GL_CW,
			WM_COUNTER_CLOCKWISE = GL_CCW
		};

	public:
		static RenderState * Get()
		{
			if(_instance == nullptr)
				_instance = new RenderState();

			return _instance;
		}
		~RenderState();

	private:

		static RenderState *	_instance;
		RenderState();

	public:

		void	clearRenderBuffer(int mode) const;
		void	setClearColor(const Vector4 & clearColor) const;

		void	setBlendmode(const BlendMode mode);

		void	setRenderSize(int width, int height);
		void	setViewport(const Vector4 & viewport) const;

		void	setDepthTest(bool depthTest);
		void	setDepthWrite(bool depthWrite);
		void	setDepthRange(float zNear, float zFar);

		void	setScissorTest(bool scissorTest);
		void	setScissorRect(const Vector4 & clip);

		void	setCullMode(const CullMode cullMode);

		void	setWindingMode(const WindingMode windingMode);

		void	bindTexture2d(uint samplerID, const Texture2D & texture);
		void	bindShader(const Shader & shader);

		void	setViewMatrix(const Matrix4 & viewMatrix);
		const Matrix4 & getViewMatrix() const;
		
		void	setProjectionMatrix(const Matrix4 & projectionMatrix);
		const Matrix4 & getProjectionMatrix() const;

		void	setModelMatrix(const Matrix4 & modelMatrix);
		const Matrix4 & getModelMatrix() const;

		/*void	bindFrameBuffer(const uint & frameBuffer);
		void	bindFrameBuffer(const FrameBufferObject & frameBuffer);*/

	private:
		typedef std::unordered_map< uint, uint > TextureCache;

		uint				_cachedShader;

		TextureCache		_cachedTextures;

		BlendMode			_cachedBlendMode;

		bool				_cachedDepthTest;

		uint				_cachedFrameBuffer;

		CullMode			_cachedCullMode;

		WindingMode			_cachedWindingMode;

		Matrix4				_modelMatrix;
		Matrix4				_viewMatrix;
		Matrix4				_projectionMatrix;

		uint				_renderWidth;
		uint				_renderHeight;

	private:
		void				_clearCache(); 
		void				_setTexture(uint samplerId, uint texHandle);
	};
}