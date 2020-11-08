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
		WindowResizeEvent e(1280, 720);
		if (e.IsInCategory(EventCategoryWindow))
		{
			MN_TRACE(e);
		}
		if (e.IsInCategory(EventCategoryInput))
		{
			MN_TRACE(e);
		}
		MN_TRACE(WindowCloseEvent());

		while (true);
	}

}