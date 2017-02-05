// ================================================================================
// ==      This file is a part of Turbo Badger. (C) 2011-2014, Emil Segerås      ==
// ==                     See tb_core.h for more information.                    ==
// ================================================================================

#ifndef TB_RENDERER_GL_H
#define TB_RENDERER_GL_H

#include "tb_types.h"

#if defined(TB_RENDERER_GL)

#if defined(TB_RENDERER_GLES_1) && defined(TB_RENDERER_GLES_2)
#error "Both GLES_1 and GLES_2 defined"
#elif defined(TB_RENDERER_GLES_1) && defined(TB_RENDERER_GL3)
#error "Both GLES_1 and GL3 defined"
#elif defined(TB_RENDERER_GLES_2) && defined(TB_RENDERER_GL3)
#error "Both GLES_2 and GL3 defined"
#endif



#define GL_GLEXT_PROTOTYPES
#include <IwGL.h>
#include <IwGLExt.h>

#define glGenVertexArrays glGenVertexArraysOES
#define glBindVertexArray glBindVertexArrayOES
#define glDeleteVertexArrays glDeleteVertexArraysOES
#define glIsVertexArray glIsVertexArrayOES


#include "renderers/tb_renderer_batcher.h"

namespace tb {

class TBRendererGL;

class TBBitmapGL : public TBBitmap
{
public:
	TBBitmapGL(TBRendererGL *renderer);
	~TBBitmapGL();
	bool Init(int width, int height, uint32 *data);
	virtual int Width() { return m_w; }
	virtual int Height() { return m_h; }
	virtual void SetData(uint32 *data);
public:
	TBRendererGL *m_renderer;
	int m_w, m_h;
	GLuint m_texture;
};

class TBRendererGL : public TBRendererBatcher
{
public:
	TBRendererGL();

	// == TBRenderer ====================================================================

	virtual void BeginPaint(int render_target_w, int render_target_h);
	virtual void EndPaint();

	virtual TBBitmap *CreateBitmap(int width, int height, uint32 *data);

	// == TBRendererBatcher ===============================================================

	virtual void RenderBatch(Batch *batch);
	virtual void SetClipRect(const TBRect &rect);

#if defined(TB_RENDERER_GLES_2) || defined(TB_RENDERER_GL3)
private:
	GLuint LoadShader(GLenum type, const GLchar * shaderSrc);
	GLuint m_program;
	GLuint m_vao;
	GLuint m_vbo;
	GLint m_orthoLoc;
	GLint m_texLoc;
	TBBitmapGL m_white;
#endif
};

} // namespace tb

#endif // TB_RENDERER_GL
#endif // TB_RENDERER_GL_H