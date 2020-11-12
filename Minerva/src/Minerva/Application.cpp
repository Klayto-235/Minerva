#include "mnpch.h"
#include "Application.h"
#include "Log.h"
#include "Events/WindowEvent.h"

namespace Minerva {

	Application::Application()
	{

	}

	Application::~Application()
	{

	}

	void Application::run()
	{
		EventBuffer ebuf;
		ebuf.post<WindowResizeEvent>(33, 22);
		ebuf.post<WindowCloseEvent>();
		MN_TRACE(ebuf);
		WindowResizeEvent e(1280, 720);
		if (e.isInCategory(EventCategoryWindow))
		{
			MN_TRACE(e);
		}
		if (e.isInCategory(EventCategoryInput))
		{
			MN_TRACE(e);
		}
		MN_TRACE(WindowCloseEvent());

		while (true);
	}

}