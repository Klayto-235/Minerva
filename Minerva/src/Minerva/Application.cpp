#include "mnpch.h"
#include "Application.h"
#include "Log.h"

#include <glad/glad.h>


namespace Minerva
{

	Application* Application::s_instance = nullptr;

	Application::Application()
	{
		MN_CORE_ASSERT(!s_instance, "application already exists.");
		s_instance = this;

		Window::init();
		MN_CORE_INFO("Window system initialised.");
		m_window = std::unique_ptr<Window>(Window::create());

		m_ImGuiLayer = new ImGuiLayer();
		m_layerStack.pushOverlay(m_ImGuiLayer);
	}

	Application::~Application()
	{
		s_instance = nullptr;
	}

	void Application::run()
	{
		while (m_running)
		{
			glClearColor(1, 0, 1, 1);
			glClear(GL_COLOR_BUFFER_BIT);

			Window::pollEvents();
			for (auto& event : m_window->getEventBuffer())
			{
				if (event->getEventCategoryFlags() & EventCategory::EventCategoryWindow)
				{
					switch (event->getEventType())
					{
					case EventType::WindowClose:
						m_running = false;
						continue;
					}
				}
				
				for (auto it = m_layerStack.rbegin(); it != m_layerStack.rend(); ++it)
				{
					if ((*it)->onEvent(*event)) break;
				}
			}

			for (Layer* layer : m_layerStack)
				layer->onUpdate();

			m_ImGuiLayer->begin();
			for (Layer* layer : m_layerStack)
				layer->onImGuiRender();
			m_ImGuiLayer->end();

			m_window->onUpdate(); // clears events
		}
	}

	void Application::pushLayer(Layer* layer)
	{
		m_layerStack.pushLayer(layer);
	}

	void Application::pushOverlay(Layer* overlay)
	{
		m_layerStack.pushOverlay(overlay);
	}

}