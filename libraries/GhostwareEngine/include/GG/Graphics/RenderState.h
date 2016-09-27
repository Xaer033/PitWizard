/*
Module: Object to handle gl code for binding / rendering
Description:	Camera object to see the world through and apply
camera view matrix to world geometry
Author: Julian Williams
*/

#pragma once

#include <unordered_map>

#include "IwGL.h"

#include <GG/Core/Types.h>
#include <GG/Core/Vector.h>
#include <GG/Graphics/Texture.h>
#include <GG/Graphics/Shader.h>

namespace GG
{
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

	class FrameBufferObject;

	class RenderState
	{
	public:
		RenderState();
		~RenderState();

		void	clearRenderBuffer(int mode);
		void	setClearColor(const Vector4 & clearColor) const;

		void	setBlendmode(const BlendMode mode);

		void	setRenderSize(int width, int height);
		void	setViewport(const Vector4 & viewport);

		void	setDepthTesting(bool depthTest);
		void	setDepthRange(float zNear, float zFar);

		void	setCullMode(const CullMode cullMode);

		void	bindTexture2d(uint samplerID, const Texture & texture);
		void	bindShader(const Shader & shader);

		/*void	bindFrameBuffer(const uint & frameBuffer);
		void	bindFrameBuffer(const FrameBufferObject & frameBuffer);*/

	private:
		typedef std::unordered_map< uint, uint > TextureCache;

		uint				_cachedShader;

		TextureCache		_cachedTextures;

		BlendMode			_cachedBlendmode;

		bool				_cachedDepthTest;

		uint				_cachedFrameBuffer;

		CullMode			_cachedCullMode;

		uint				_renderWidth;
		uint				_renderHeight;

	private:
		void				_clearCache(); 
		void				_setTexture(uint samplerId, uint texHandle);
	};
}