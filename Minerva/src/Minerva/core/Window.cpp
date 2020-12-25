#include "mnpch.h"
#include "Minerva/core/Window.h"
#include "Minerva/Events/WindowEvent.h"
#include "Minerva/core/Application.h"
#include "Minerva/Renderer/Renderer.h"


namespace Minerva
{

	void Window::initResources()
	{
		RenderCommand::init();
		m_renderer2D = createScope<Renderer2D>();
	}

	void Window::freeResources()
	{
		deleteLayers();
		m_renderer2D.reset();
	}

	void Window::onUpdate(const float timeStep, EventBuffer& appEventBuffer)
	{
		for (auto& event : m_data.eventBuffer)
		{
			if (event->getEventCategoryFlags() & EventCategory::EventCategoryWindow)
			{
				switch (event->getEventType())
				{
				case EventType::WindowClose:
					appEventBuffer.add<WindowCloseEvent>(this);
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
		m_data.eventBuffer.clear();

		if (!m_minimised)
		{
			for (auto it = m_layerStack.rbegin(); it != m_layerStack.rend(); ++it)
			{
				(*it)->onUpdate(timeStep, getInputState());
			}

			for (auto& layer : m_layerStack)
			{
				layer->onRender(*m_renderer2D);
			}
		}
	}

	void Window::onImGuiRender()
	{
		for (auto& layer : m_layerStack)
			layer->onImGuiRender();
	}

}