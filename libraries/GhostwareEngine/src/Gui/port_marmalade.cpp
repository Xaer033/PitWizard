// -*-  Mode: C++; c-basic-offset: 4; tab-width: 4; indent-tabs-mode: t -*-

#include "AppBackendMarmalade.h"

#include <locale>
#include <string>
#include "tb_skin.h"
#include "tb_system.h"
#include "tb_msg.h"
#include "tb_font_renderer.h"
#include "Application.h"
#include "tb_animation.h"


#include <Gui/tb_renderer_gl.h>
#include <s3e.h>
#include <s3eKeyboard.h>
#include <s3ePointer.h>

using namespace tb;



MODIFIER_KEYS GetModifierKeys()
{
	MODIFIER_KEYS code = TB_MODIFIER_NONE;

	if(s3eKeyboardGetState(s3eKeyLeftAlt) & s3eKeyStateDown ||
		s3eKeyboardGetState(s3eKeyLeftAlt) & s3eKeyStateDown)
	{
		code |= TB_ALT;
	}

	if(s3eKeyboardGetState(s3eKeyLeftControl) & s3eKeyStateDown || 
		s3eKeyboardGetState(s3eKeyRightControl) & s3eKeyStateDown)
	{
		code |= TB_CTRL;
	}

	if(s3eKeyboardGetState(s3eKeyLeftShift) & s3eKeyStateDown ||
		s3eKeyboardGetState(s3eKeyRightShift) & s3eKeyStateDown)
	{
		code |= TB_SHIFT;
	}
	//if (mods & KMOD_GUI)	code |= TB_SUPER; // no idea what SUPER means, but doesn't seem to be used
	return code;
}

static bool ShouldEmulateTouchEvent()
{
	// Used to emulate that mouse events are touch events when alt, ctrl and shift are pressed.
	// This makes testing a lot easier when there is no touch screen around :)
	return (GetModifierKeys() & (TB_ALT | TB_CTRL | TB_SHIFT)) ? true : false;
}

// @return Return the upper case of a ascii charcter. Only for shortcut handling.
static int toupr_ascii(int ascii)
{
	if (ascii >= 'a' && ascii <= 'z')
		return ascii + 'A' - 'a';
	return ascii;
}

static bool InvokeShortcut(int key, SPECIAL_KEY special_key, MODIFIER_KEYS modifierkeys, bool down)
{
#ifdef TB_TARGET_MACOSX
	bool shortcut_key = (modifierkeys & TB_SUPER) ? true : false;
#else
	bool shortcut_key = (modifierkeys & TB_CTRL) ? true : false;
#endif
	if (!TBWidget::focused_widget || !down || !shortcut_key)
		return false;
	bool reverse_key = (modifierkeys & TB_SHIFT) ? true : false;
	int upper_key = toupr_ascii(key);
	TBID id;
	if (upper_key == 'X')
		id = TBIDC("cut");
	else if (upper_key == 'C' || special_key == TB_KEY_INSERT)
		id = TBIDC("copy");
	else if (upper_key == 'V' || (special_key == TB_KEY_INSERT && reverse_key))
		id = TBIDC("paste");
	else if (upper_key == 'A')
		id = TBIDC("selectall");
	else if (upper_key == 'Z' || upper_key == 'Y')
	{
		bool undo = upper_key == 'Z';
		if (reverse_key)
			undo = !undo;
		id = undo ? TBIDC("undo") : TBIDC("redo");
	}
	else if (upper_key == 'N')
		id = TBIDC("new");
	else if (upper_key == 'O')
		id = TBIDC("open");
	else if (upper_key == 'S')
		id = TBIDC("save");
	else if (upper_key == 'W')
		id = TBIDC("close");
	else if (special_key == TB_KEY_PAGE_UP)
		id = TBIDC("prev_doc");
	else if (special_key == TB_KEY_PAGE_DOWN)
		id = TBIDC("next_doc");
	else
		return false;

	TBWidgetEvent ev(EVENT_TYPE_SHORTCUT);
	ev.modifierkeys = modifierkeys;
	ev.ref_id = id;
	return TBWidget::focused_widget->InvokeEvent(ev);
}

bool AppBackendMarmalade::InvokeKey(unsigned int key, SPECIAL_KEY special_key, MODIFIER_KEYS modifierkeys, bool down)
{/*
	tb::TBStr str;
	str.SetFormatted("Key: %d, Special: %d, Modifiers: %d, Down: %d", key, special_key, modifierkeys, (uint)down);
	TBDebugPrint(str.CStr());*/

	if (InvokeShortcut(key, special_key, modifierkeys, down))
		return true;
	return m_app->GetRoot()->InvokeKey(key, special_key, modifierkeys, down);
}


bool AppBackendMarmalade::HandleSpecialKey(s3eKey key, tb::MODIFIER_KEYS modifier, bool down, AppBackendMarmalade * backend)
{
	switch(key)
	{
	case s3eKeyF1:			return backend->InvokeKey(0, TB_KEY_F1, modifier, down); break;
	case s3eKeyF2:			return backend->InvokeKey(0, TB_KEY_F2, modifier, down); break;
	case s3eKeyF3:			return backend->InvokeKey(0, TB_KEY_F3, modifier, down); break;
	case s3eKeyF4:			return backend->InvokeKey(0, TB_KEY_F4, modifier, down); break;
	case s3eKeyF5:			return backend->InvokeKey(0, TB_KEY_F5, modifier, down); break;
	case s3eKeyF6:			return backend->InvokeKey(0, TB_KEY_F6, modifier, down); break;
	case s3eKeyF7:			return backend->InvokeKey(0, TB_KEY_F7, modifier, down); break;
	case s3eKeyF8:			return backend->InvokeKey(0, TB_KEY_F8, modifier, down); break;
	case s3eKeyF9:			return backend->InvokeKey(0, TB_KEY_F9, modifier, down); break;
	case s3eKeyF10:			return backend->InvokeKey(0, TB_KEY_F10, modifier, down); break;
	case s3eKeyF11:			return backend->InvokeKey(0, TB_KEY_F11, modifier, down); break;
	case s3eKeyF12:			return backend->InvokeKey(0, TB_KEY_F12, modifier, down); break;
	case s3eKeyLeft:		return backend->InvokeKey(0, TB_KEY_LEFT, modifier, down); break;
	case s3eKeyUp:			return backend->InvokeKey(0, TB_KEY_UP, modifier, down); break;
	case s3eKeyRight:		return backend->InvokeKey(0, TB_KEY_RIGHT, modifier, down); break;
	case s3eKeyDown:		return backend->InvokeKey(0, TB_KEY_DOWN, modifier, down); break;
	case s3eKeyPageUp:		return backend->InvokeKey(0, TB_KEY_PAGE_UP, modifier, down); break;
	case s3eKeyPageDown:	return backend->InvokeKey(0, TB_KEY_PAGE_DOWN, modifier, down); break;
	case s3eKeyHome:		return backend->InvokeKey(0, TB_KEY_HOME, modifier, down); break;
	case s3eKeyEnd:			return backend->InvokeKey(0, TB_KEY_END, modifier, down); break;
	case s3eKeyInsert:		return backend->InvokeKey(0, TB_KEY_INSERT, modifier, down); break;
	//case s3eKeyTab:			return backend->InvokeKey(0, TB_KEY_TAB, modifier, down); break;
	case s3eKeyDelete:		return backend->InvokeKey(0, TB_KEY_DELETE, modifier, down); break;
	//case s3eKeyBackspace:	return backend->InvokeKey(0, TB_KEY_BACKSPACE, modifier, down); break;
	case s3eKeyEnter:		return backend->InvokeKey(0, TB_KEY_ENTER, modifier, down); break;
	case s3eKeyEsc:			return backend->InvokeKey(0, TB_KEY_ESC, modifier, down); break;
	case s3eKeyMenu:
		if(TBWidget::focused_widget && !down)
		{
			TBWidgetEvent ev(EVENT_TYPE_CONTEXT_MENU);
			ev.modifierkeys = modifier;
			TBWidget::focused_widget->InvokeEvent(ev);
		}
		break;
	default:
		return false;
	}
	return true;
}

void AppBackendMarmalade::KeyboardCharCallback(s3eKeyboardCharEvent * e, void * userData)
{
	AppBackendMarmalade * app = (AppBackendMarmalade*)userData;

	uint utf = 0;
	utf8::encode(e->m_Char, (char*)&utf);

	if(utf == '\b')
	{
		app->InvokeKey(0, TB_KEY_BACKSPACE, GetModifierKeys(), true);
		app->InvokeKey(0, TB_KEY_BACKSPACE, GetModifierKeys(), false);
	}
	else if(utf == '\t')
	{
		app->InvokeKey(0, TB_KEY_TAB, GetModifierKeys(), true);
		app->InvokeKey(0, TB_KEY_TAB, GetModifierKeys(), false);
	}
	else if(utf == 13)
	{

	}
	else
	{
		app->InvokeKey(utf, TB_KEY_UNDEFINED, GetModifierKeys(), true);
		app->InvokeKey(utf, TB_KEY_UNDEFINED, GetModifierKeys(), false);
	}
}

void AppBackendMarmalade::KeyboardCallback(s3eKeyboardEvent * e, void * userData)
{
	AppBackendMarmalade * app = (AppBackendMarmalade*)userData;
	HandleSpecialKey(e->m_Key, GetModifierKeys(), e->m_Pressed != 0, app);
}

void AppBackendMarmalade::TouchMotionCallback(s3ePointerMotionEvent * e, void * userData)
{
	App * app = (App*)userData;
	CIwGLPoint p = FilterTouchPosition(e->m_x, e->m_y, app->GetWidth(), app->GetHeight());
	app->GetRoot()->InvokePointerMove(p.x, p.y, GetModifierKeys(), ShouldEmulateTouchEvent());
}

void AppBackendMarmalade::ResizeCallback(s3eSurfaceOrientation * e, void * userData)
{
	App * app = (App*)userData;
	app->GetRoot()->OnResized(e->m_Width, e->m_Height);
}

CIwGLPoint AppBackendMarmalade::TransformPoint(int x, int y)
{
	return IwGLTransform(CIwGLPoint(x, y));
}


CIwGLPoint AppBackendMarmalade::FilterTouchPosition(int x, int y, uint width, uint height)
{
	const float kWiggleRoom = 100;
	static CIwGLPoint oldPos = CIwGLPoint(0, 0);

	CIwGLPoint pos = TransformPoint(x, y);
	if(pos.x == 0 && oldPos.x >= width - kWiggleRoom)
	{
		pos.x = oldPos.x;
	}

	if(pos.y == 0 && oldPos.y >= height - kWiggleRoom)
	{
		pos.y = oldPos.y;
	}

	oldPos = pos;
	return pos;
}

void AppBackendMarmalade::TouchCallback(s3ePointerEvent * e, void * userData)
{
	static double last_time = 0;
	static int last_x = 0;
	static int last_y = 0;
	static int counter = 1;

	App * app = (App*)userData;
	CIwGLPoint p = FilterTouchPosition(e->m_x, e->m_y, app->GetWidth(), app->GetHeight());
	int x = p.x;
	int y = p.y;

	double time = TBSystem::GetTimeMS();
	if (time < last_time + 600 && abs(last_x - x) < 10 && abs(last_y - y) < 10)
		counter++;
	else
		counter = 1;

	last_x = x;
	last_y = y;
	last_time = time;

	switch(e->m_Button)
	{
	case S3E_POINTER_BUTTON_LEFTMOUSE:
	{
		if(e->m_Pressed)
			app->GetRoot()->InvokePointerDown(x, y, counter, GetModifierKeys(), ShouldEmulateTouchEvent());
		else
			app->GetRoot()->InvokePointerUp(x, y, GetModifierKeys(), ShouldEmulateTouchEvent());
		break;
	}
	case S3E_POINTER_BUTTON_RIGHTMOUSE:

		break;
	case S3E_POINTER_BUTTON_MIDDLEMOUSE:
		break;
	default:
		break;
	}
}

bool AppBackendMarmalade::Init(App *app)
{
	//Initialise graphics system(s)
	if(!IwGLInit())
	{
		return false;
	}

	IwGLSwapBuffers();
	s3eDeviceYield(0);
	
	uint width	= IwGLGetInt(IW_GL_WIDTH);
	uint height	= IwGLGetInt(IW_GL_HEIGHT);

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	m_renderer = new TBRendererGL();
	tb_core_init(m_renderer);

	// Create the App object for our demo
	m_app = app;
	m_app->OnBackendAttached(this, width, height);


	//Register for standard pointer events
	s3ePointerRegister(S3E_POINTER_BUTTON_EVENT, (s3eCallback)TouchCallback, (void*)m_app);
	s3ePointerRegister(S3E_POINTER_MOTION_EVENT, (s3eCallback)TouchMotionCallback, (void*)m_app);
	s3eKeyboardRegister(S3E_KEYBOARD_KEY_EVENT, (s3eCallback)KeyboardCallback, (void*)this);
	s3eKeyboardRegister(S3E_KEYBOARD_CHAR_EVENT, (s3eCallback)KeyboardCharCallback, (void*)this);
	s3eSurfaceRegister(S3E_SURFACE_SCREENSIZE, (s3eCallback)ResizeCallback, (void*)m_app);
	s3eKeyboardSetInt(S3E_KEYBOARD_GET_CHAR, 1);

	return true;
}

AppBackendMarmalade::~AppBackendMarmalade()
{
	s3eKeyboardSetInt(S3E_KEYBOARD_GET_CHAR, 0);
	s3ePointerUnRegister(S3E_POINTER_BUTTON_EVENT, (s3eCallback)TouchCallback);
	s3ePointerUnRegister(S3E_POINTER_MOTION_EVENT, (s3eCallback)TouchMotionCallback);
	s3eKeyboardUnRegister(S3E_KEYBOARD_KEY_EVENT, (s3eCallback)KeyboardCallback);
	s3eKeyboardUnRegister(S3E_KEYBOARD_CHAR_EVENT, (s3eCallback)KeyboardCharCallback);
	s3eSurfaceUnRegister(S3E_SURFACE_SCREENSIZE, (s3eCallback)ResizeCallback);

	m_app->OnBackendDetached();
	m_app = nullptr;

	tb_core_shutdown();

	delete m_renderer;
}

void AppBackendMarmalade::OnAppEvent(const EVENT &ev)
{
	switch (ev)
	{
	case EVENT_PAINT_REQUEST:
		if (!m_has_pending_update)
		{
			m_has_pending_update = true;
		}
		break;
	case EVENT_QUIT_REQUEST:
		m_quit_requested = true;
		break;
	case EVENT_TITLE_CHANGED:
		//SDL_SetWindowTitle(mainWindow, m_app->GetTitle());
		
		break;
	default:
		assert(!"Unhandled app event!");
	}
}

// Attempt to convert an sdl event to a TB event, return true if handled
//bool
//AppBackendMarmalade::HandleMarmaladeEvent()
//{
//	bool handled = true;
//	switch (event.type) {
//	case SDL_KEYUP:
//	case SDL_KEYDOWN: {
//		// SDL_KeyboardEvent
//		// Handle any key presses that wont also be SDL_TEXTINPUT here.
//		bool down = event.type == SDL_KEYDOWN;
//		MODIFIER_KEYS modifier = GetModifierKeys((SDL_Keymod)event.key.keysym.mod);
//		// handle keys
//		switch (event.key.keysym.sym)
//		{
//		case SDLK_F1:			InvokeKey(0, TB_KEY_F1, modifier, down); break;
//		case SDLK_F2:			InvokeKey(0, TB_KEY_F2, modifier, down); break;
//		case SDLK_F3:			InvokeKey(0, TB_KEY_F3, modifier, down); break;
//		case SDLK_F4:			InvokeKey(0, TB_KEY_F4, modifier, down); break;
//		case SDLK_F5:			InvokeKey(0, TB_KEY_F5, modifier, down); break;
//		case SDLK_F6:			InvokeKey(0, TB_KEY_F6, modifier, down); break;
//		case SDLK_F7:			InvokeKey(0, TB_KEY_F7, modifier, down); break;
//		case SDLK_F8:			InvokeKey(0, TB_KEY_F8, modifier, down); break;
//		case SDLK_F9:			InvokeKey(0, TB_KEY_F9, modifier, down); break;
//		case SDLK_F10:			InvokeKey(0, TB_KEY_F10, modifier, down); break;
//		case SDLK_F11:			InvokeKey(0, TB_KEY_F11, modifier, down); break;
//		case SDLK_F12:			InvokeKey(0, TB_KEY_F12, modifier, down); break;
//		case SDLK_LEFT:			InvokeKey(0, TB_KEY_LEFT, modifier, down); break;
//		case SDLK_UP:			InvokeKey(0, TB_KEY_UP, modifier, down); break;
//		case SDLK_RIGHT:		InvokeKey(0, TB_KEY_RIGHT, modifier, down); break;
//		case SDLK_DOWN:			InvokeKey(0, TB_KEY_DOWN, modifier, down); break;
//		case SDLK_PAGEUP:		InvokeKey(0, TB_KEY_PAGE_UP, modifier, down); break;
//		case SDLK_PAGEDOWN:		InvokeKey(0, TB_KEY_PAGE_DOWN, modifier, down); break;
//		case SDLK_HOME:			InvokeKey(0, TB_KEY_HOME, modifier, down); break;
//		case SDLK_END:			InvokeKey(0, TB_KEY_END, modifier, down); break;
//		case SDLK_INSERT:		InvokeKey(0, TB_KEY_INSERT, modifier, down); break;
//		case SDLK_TAB:			InvokeKey(0, TB_KEY_TAB, modifier, down); break;
//		case SDLK_DELETE:		InvokeKey(0, TB_KEY_DELETE, modifier, down); break;
//		case SDLK_BACKSPACE:	InvokeKey(0, TB_KEY_BACKSPACE, modifier, down); break;
//		case SDLK_RETURN:
//		case SDLK_KP_ENTER:		InvokeKey(0, TB_KEY_ENTER, modifier, down); break;
//		case SDLK_ESCAPE:		InvokeKey(0, TB_KEY_ESC, modifier, down); break;
//		case SDLK_MENU:
//			if (TBWidget::focused_widget && !down)
//			{
//				TBWidgetEvent ev(EVENT_TYPE_CONTEXT_MENU);
//				ev.modifierkeys = modifier;
//				TBWidget::focused_widget->InvokeEvent(ev);
//			}
//			break;
//			/* just ignore lone modifier key presses */
//		case SDLK_LCTRL:
//		case SDLK_RCTRL:
//		case SDLK_LALT:
//		case SDLK_RALT:
//		case SDLK_LSHIFT:
//		case SDLK_RSHIFT:
//		case SDLK_LGUI:
//		case SDLK_RGUI:
//			break;
//			/* also ignore everything else, since it should show up in SDL_TEXTINPUT */
//		case SDLK_PLUS:
//		case SDLK_MINUS:
//		default:
//			// skip key events that will appear as SDL_TEXTINPUT
//			if (modifier & (TB_ALT | TB_CTRL)) {
//				unsigned int character = event.key.keysym.sym;
//				if (character >= 'a' && character <= 'z' && (modifier & TB_SHIFT))
//					character += ('A' - 'a');
//				InvokeKey(character, TB_KEY_UNDEFINED, modifier, down);
//			}
//			handled = true;
//			break;
//		}
//		break;
//	}
//	case SDL_FINGERMOTION:
//	case SDL_FINGERDOWN:
//	case SDL_FINGERUP:
//		//event.tfinger;
//		break;
//
//	case SDL_MOUSEMOTION: {
//		if (m_app->GetRoot() && !(ShouldEmulateTouchEvent() && !TBWidget::captured_widget))
//			m_app->GetRoot()->InvokePointerMove(event.motion.x, event.motion.y, 
//												GetModifierKeys(),
//												ShouldEmulateTouchEvent());
//
//		break;
//	}
//	case SDL_MOUSEBUTTONUP:
//	case SDL_MOUSEBUTTONDOWN: {
//		// Handle mouse clicks here.
//		MODIFIER_KEYS modifier = GetModifierKeys();
//		int x = event.button.x;
//		int y = event.button.y;
//		if (event.button.button == SDL_BUTTON_LEFT)
//		{
//			if (event.type == SDL_MOUSEBUTTONDOWN)
//			{
//				// This is a quick fix with n-click support :)
//				static double last_time = 0;
//				static int last_x = 0;
//				static int last_y = 0;
//				static int counter = 1;
//
//				double time = TBSystem::GetTimeMS();
//				if (time < last_time + 600 && last_x == x && last_y == y)
//					counter++;
//				else
//					counter = 1;
//				last_x = x;
//				last_y = y;
//				last_time = time;
//
//				m_app->GetRoot()->InvokePointerDown(x, y, counter, modifier, ShouldEmulateTouchEvent());
//			}
//			else
//				m_app->GetRoot()->InvokePointerUp(x, y, modifier, ShouldEmulateTouchEvent());
//		}
//		else if (event.button.button == SDL_BUTTON_RIGHT && event.type == SDL_MOUSEBUTTONUP)
//		{
//			m_app->GetRoot()->InvokePointerMove(x, y, modifier, ShouldEmulateTouchEvent());
//			if (TBWidget::hovered_widget)
//			{
//				TBWidget::hovered_widget->ConvertFromRoot(x, y);
//				TBWidgetEvent ev(EVENT_TYPE_CONTEXT_MENU, x, y, false, modifier);
//				TBWidget::hovered_widget->InvokeEvent(ev);
//			}
//		}
//	}
//		break;
//	case SDL_MOUSEWHEEL: {
//		int mouse_x, mouse_y;
//		SDL_GetMouseState(&mouse_x, &mouse_y);
//		if (m_app->GetRoot())
//			m_app->GetRoot()->InvokeWheel(mouse_x, mouse_y,
//										  (int)event.wheel.x, -(int)event.wheel.y,
//										  GetModifierKeys());
//		break;
//	}
//	case SDL_MULTIGESTURE:
//		//event.mgesture;
//		break;
//	case SDL_SYSWMEVENT:
//		//event.syswm;
//		break;
//	case SDL_TEXTEDITING:
//		//event.edit;
//		break;
//	case SDL_TEXTINPUT: {
//		MODIFIER_KEYS modifier = GetModifierKeys();
//		for (int ii = 0; event.text.text[ii]; ii++) {
//			unsigned int character = event.text.text[ii];
//			InvokeKey(character, TB_KEY_UNDEFINED, modifier, true);
//			InvokeKey(character, TB_KEY_UNDEFINED, modifier, false);
//		}
//		break;
//	}
//	case SDL_WINDOWEVENT: {
//		switch (event.window.event) {
//		case SDL_WINDOWEVENT_SHOWN:
//			//SDL_Log("Window %d shown", event.window.windowID);
//			break;
//		case SDL_WINDOWEVENT_HIDDEN:
//			//SDL_Log("Window %d hidden", event.window.windowID);
//			break;
//		case SDL_WINDOWEVENT_EXPOSED:
//			//SDL_Log("Window %d exposed", event.window.windowID);
//			OnAppEvent(EVENT_PAINT_REQUEST);
//			break;
//		case SDL_WINDOWEVENT_MOVED:
//			//SDL_Log("Window %d moved to %d,%d",
//			//		event.window.windowID, event.window.data1,
//			//		event.window.data2);
//			break;
//		case SDL_WINDOWEVENT_RESIZED:
//			if (m_app)
//				m_app->OnResized(event.window.data1, event.window.data2);
//			//SDL_Log("Window %d resized to %dx%d",
//			//		event.window.windowID, event.window.data1,
//			//		event.window.data2);
//			break;
//		case SDL_WINDOWEVENT_SIZE_CHANGED:
//			//SDL_Log("Window %d size changed to %dx%d",
//			//		event.window.windowID, event.window.data1,
//			//		event.window.data2);
//			break;
//		case SDL_WINDOWEVENT_MINIMIZED:
//			//SDL_Log("Window %d minimized", event.window.windowID);
//			break;
//		case SDL_WINDOWEVENT_MAXIMIZED:
//			//SDL_Log("Window %d maximized", event.window.windowID);
//			break;
//		case SDL_WINDOWEVENT_RESTORED:
//			//SDL_Log("Window %d restored", event.window.windowID);
//			break;
//		case SDL_WINDOWEVENT_ENTER:
//			//SDL_Log("Mouse entered window %d", event.window.windowID);
//			break;
//		case SDL_WINDOWEVENT_LEAVE:
//			//SDL_Log("Mouse left window %d", event.window.windowID);
//			break;
//		case SDL_WINDOWEVENT_FOCUS_GAINED:
//			//SDL_Log("Window %d gained keyboard focus", event.window.windowID);
//			break;
//		case SDL_WINDOWEVENT_FOCUS_LOST:
//			//SDL_Log("Window %d lost keyboard focus", event.window.windowID);
//			break;
//		case SDL_WINDOWEVENT_CLOSE:
//			//SDL_Log("Window %d closed", event.window.windowID);
//			break;
//		default:
//			handled = false;
//			SDL_Log("Window %d got unknown event %d", event.window.windowID, event.window.event);
//			break;
//		}
//		break;
//	}
//	case SDL_USEREVENT:
//		// event.user;
//		// draw event
//		if (m_has_pending_update)
//		{
//			m_app->Process();
//			m_has_pending_update = false;
//			// Bail out if we get here with invalid dimensions.
//			// This may happen when minimizing windows (GLFW 3.0.4, Windows 8.1).
//			if (GetWidth() == 0 || GetHeight() == 0)
//				; // ignore
//			else
//			{
//				m_app->RenderFrame();
//				SDL_GL_SwapWindow(mainWindow);
//			}
//		}
//		break;
//	case SDL_QUIT:
//		m_quit_requested = true;
//		return true;
//	default:
//		handled = false;
//	}
//	return handled;
//}

bool port_main()
{
	App *app = app_create();

	AppBackendMarmalade *backend = new AppBackendMarmalade();
	if (!backend || !backend->Init(app))
		return false;

	bool success = app->Init();
	if (success)
	{
		// Loop forever, until the user or the OS performs some action to quit the app
		while(!s3eDeviceCheckQuitRequest())
		{
			//Update the input systems
			s3eKeyboardUpdate();
			s3ePointerUpdate();

			app->Process();

			// Get dimensions from IwGL
			int w = IwGLGetInt(IW_GL_VIRTUAL_WIDTH);
			int h = IwGLGetInt(IW_GL_VIRTUAL_WIDTH);

			glViewport(0, 0, w, h);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

			// Your rendering/app code goes here.
			app->RenderFrame();
			// Call IwGL swap instead of egl directly
			IwGLSwapBuffers();

			// Sleep for 0ms to allow the OS to process events etc.
			s3eDeviceYield(0);
		}
		app->ShutDown();
	}

	delete backend;
	delete app;

	return success;
}


//int main(int argc, char** argv)
//{
//	return port_main() ? 0 : 1;
//}
