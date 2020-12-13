#include "mnpch.h"
#include "Application.h"
#include "Renderer/Renderer.h"
#include "Minerva/Events/WindowEvent.h"

#include <GLFW/glfw3.h> // TEMPORARY (timestep)


namespace Minerva
{

	Application* Application::s_instance = nullptr;

	Application::Application()
	{
		MN_CORE_ASSERT(!s_instance, "Application::Application: Application already exists.");
		s_instance = this;

		m_window = std::unique_ptr<Window>(Window::create());

		Renderer::init();

		m_ImGuiLayer = new ImGuiLayer();
		m_layerStack.pushOverlay(m_ImGuiLayer);
	}

	Application::~Application()
	{
		s_instance = nullptr;
	}

	void Application::run()
	{
		m_running = true;
		m_lastFrameTime = static_cast<float>(glfwGetTime());

		while (m_running)
		{
			const float currentTime = static_cast<float>(glfwGetTime());
			const float deltaTime = currentTime - m_lastFrameTime;
			m_lastFrameTime = currentTime;

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
					case EventType::WindowResize:
						const WindowResizeEvent& e = static_cast<const WindowResizeEvent&>(*event);
						if (e.getWidth() == 0 || e.getHeight() == 0) m_minimised = true;
						else m_minimised = false;
						Renderer::onWindowResize(e.getWidth(), e.getHeight());
					}
				}
				
				for (auto it = m_layerStack.rbegin(); it != m_layerStack.rend(); ++it)
				{
					if ((*it)->onEvent(*event)) break;
				}
			}

			if (!m_minimised) {
				for (Layer* layer : m_layerStack)
					layer->onUpdate(deltaTime);
			}

			m_ImGuiLayer->begin();
			for (Layer* layer : m_layerStack)
				layer->onImGuiRender();
			m_ImGuiLayer->end();

			m_window->onUpdate(); // clears internal event buffer
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