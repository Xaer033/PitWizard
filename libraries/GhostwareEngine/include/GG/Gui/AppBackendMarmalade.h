#pragma once

#include <s3e.h>
#include <s3eKeyboard.h>
#include <s3ePointer.h>
#include <IwGL.h>


//#include <TurboBadger/src/Demo.h>
#include <tb/tb_widgets.h>
#include <Gui/tb_renderer_gl.h>
#include <Gui/Application.h>

class AppBackendMarmalade : public AppBackend
{
public:
	bool Init(App *app);
	AppBackendMarmalade() : m_quit_requested(false)
		, m_app(nullptr)
		, m_renderer(nullptr)
		, m_has_pending_update(false) {}
	~AppBackendMarmalade();

	virtual void OnAppEvent(const EVENT &ev);

	tb::TBWidget *GetRoot() const { return m_app->GetRoot(); }
	int GetWidth() const { return m_app->GetWidth(); }
	int GetHeight() const { return m_app->GetHeight(); }

	//bool HandleSDLEvent(SDL_Event & event);
	bool m_quit_requested;
private:
	static bool		HandleSpecialKey(s3eKey key, tb::MODIFIER_KEYS modifier, bool down, AppBackendMarmalade * backend);
	static CIwGLPoint FilterTouchPosition(int x, int y, uint width, uint height);

	static void		KeyboardCallback(s3eKeyboardEvent * e, void * userData);
	static void		KeyboardCharCallback(s3eKeyboardCharEvent * e, void * userData);
	static void		TouchCallback(s3ePointerEvent * e, void * userData);
	static void		TouchMotionCallback(s3ePointerMotionEvent * e, void * userData);
	static void		ResizeCallback(s3eSurfaceOrientation * e, void * userData);

	static CIwGLPoint TransformPoint(int x, int y);

	bool InvokeKey(unsigned int key,tb::SPECIAL_KEY special_key,
		tb::MODIFIER_KEYS modifierkeys, bool down);

	App *m_app;
	tb::TBRendererGL *m_renderer;
	bool m_has_pending_update;
};
