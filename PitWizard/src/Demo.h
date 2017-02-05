#ifndef DEMO_H
#define DEMO_H

#include "tb_widgets.h"
#include "tb_widgets_common.h"
#include "tb_widgets_reader.h"
#include "tb_widgets_listener.h"
#include "tb_message_window.h"
#include "tb_msg.h"
#include "tb_scroller.h"

#include <Gui/Application.h>

using namespace tb;

class DemoApplication : public App
{
public:
	DemoApplication() : App(1280, 700) {}

	virtual const char *GetTitle() const { return "Demo"; }
	virtual void OnBackendAttached(AppBackend *backend, int width, int height);
	virtual bool Init();
	virtual void RenderFrame();
};

class DemoWindow : public TBWindow
{
public:
	DemoWindow(TBWidget *root);
	bool LoadResourceFile(const char *filename);
	void LoadResourceData(const char *data);
	void LoadResource(TBNode &node);

	virtual bool OnEvent(const TBWidgetEvent &ev);
};

class MainWindow : public DemoWindow, public TBMessageHandler
{
public:
	MainWindow(TBWidget *root);
	virtual bool OnEvent(const TBWidgetEvent &ev);

	// Implement TBMessageHandler
	virtual void OnMessageReceived(TBMessage *msg);
};


#endif // DEMO_H
