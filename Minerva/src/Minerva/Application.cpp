#include "mnpch.h"
#include "Application.h"
#include "Log.h"
#include "Events/event.h"

#include <GLFW/glfw3.h>


namespace Minerva
{

	Application::Application()
	{
		Window::init();
		MN_CORE_INFO("Window system initialised.");
		m_window = std::unique_ptr<Window>(Window::create());
	}

	Application::~Application()
	{

	}

	void Application::run()
	{
		while (true)
		{
			Window::pollEvents();
			MN_CORE_TRACE(m_window->getEventBuffer());
			glClearColor(1, 0, 1, 1);
			glClear(GL_COLOR_BUFFER_BIT);
			m_window->onUpdate();
		}
	}

}