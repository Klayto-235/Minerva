#include "mnpch.h"
#include "Minerva/core/Window.h"
#include "Minerva/Events/WindowEvent.h"
#include "Minerva/core/Application.h"
#include "Minerva/Renderer/Renderer.h"


namespace Minerva
{

	Window::~Window()
	{
		MN_PROFILE_FUNCTION();

		MN_CORE_INFO("Destroying window \"{0}\".", m_data.title);
		m_graphicsContext->makeCurrent();
		deleteLayers();
		m_renderer2D.reset();

		impl_destroyWindow();
	}

	void Window::deleteLayer(Layer* layer)
	{
		MN_PROFILE_FUNCTION();

		MN_CORE_ASSERT(!g_lockWindows, "Window::deleteLayer: Cannot delete layer while windows are locked.");
		auto it = std::find_if(m_layers.begin(), m_layers.end(),
			[layer](const Scope<Layer>& other) { return layer == other.get(); });
		MN_CORE_ASSERT(it != m_layers.end(), "Window::deleteLayer: Could not find layer.");
		m_graphicsContext->makeCurrent();
		removeLayer(layer);
		m_layers.erase(it);
	}

	void Window::deleteLayers()
	{
		MN_PROFILE_FUNCTION();

		MN_CORE_ASSERT(!g_lockWindows, "Window::deleteLayers: Cannot delete layers while windows are locked.");
		m_graphicsContext->makeCurrent();
		for (auto& layer : m_layers)
		{
			m_layerStack.removeOverlay(layer.get());
			m_layerStack.removeLayer(layer.get());
		}
		m_layers.clear();
	}

	void Window::pushLayer(Layer* layer)
	{
		MN_PROFILE_FUNCTION();

		MN_CORE_ASSERT(!g_lockWindows, "Window::pushLayer: Cannot push layer while windows are locked.");
		MN_CORE_ASSERT(layer->m_owner == nullptr || layer->m_owner == this,
			"Window::pushLayer: Cannot push layer that is owned by another window.");
		m_layerStack.pushLayer(layer);
	}

	void Window::pushOverlay(Layer* overlay)
	{
		MN_PROFILE_FUNCTION();

		MN_CORE_ASSERT(!g_lockWindows, "Window::pushOverlay: Cannot push overlay while windows are locked.");
		MN_CORE_ASSERT(overlay->m_owner == nullptr || overlay->m_owner == this,
			"Window::pushOverlay: Cannot push overlay that is owned by another window.");
		m_layerStack.pushOverlay(overlay);
	}

	Layer* Window::popLayer()
	{
		MN_PROFILE_FUNCTION();

		MN_CORE_ASSERT(!g_lockWindows, "Window::popLayer: Cannot pop layer while windows are locked.");
		return m_layerStack.popLayer();
	}

	Layer* Window::popOverlay()
	{
		MN_PROFILE_FUNCTION();

		MN_CORE_ASSERT(!g_lockWindows, "Window::popOverlay: Cannot pop overlay while windows are locked.");
		return m_layerStack.popOverlay();
	}

	size_t Window::removeLayer(Layer* layer)
	{
		MN_PROFILE_FUNCTION();

		MN_CORE_ASSERT(!g_lockWindows, "Window::removeLayer: Cannot remove layer while windows are locked.");
		return m_layerStack.removeLayer(layer);
	}

	size_t Window::removeOverlay(Layer* overlay)
	{
		MN_PROFILE_FUNCTION();

		MN_CORE_ASSERT(!g_lockWindows, "Window::removeOverlay: Cannot remove overlay while windows are locked.");
		return m_layerStack.removeOverlay(overlay);
	}

	void Window::clearLayerStack()
	{
		MN_PROFILE_FUNCTION();

		MN_CORE_ASSERT(!g_lockWindows, "Window::clearLayerStack: Cannot clear layer stack while windows are locked.");
		m_layerStack.clear();
	}

	Window::Window(const WindowProperties& properties)
		: m_data{ properties.title, properties.width, properties.height }
	{
		MN_PROFILE_FUNCTION();

		MN_CORE_INFO("Creating window \"{0}\" ({1} x {2}).", m_data.title, m_data.width, m_data.height);
		impl_createWindow(); // implicit m_graphicsContext->makeCurrent()

		Renderer::init();
		m_renderer2D = createScope<Renderer2D>();
	}

	void Window::onUpdate(const float timeStep, EventBuffer& appEventBuffer)
	{
		MN_PROFILE_FUNCTION();

		{
			MN_PROFILE_SCOPE("events - Window::onUpdate");

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
		}

		if (!m_minimised)
		{
			{
				MN_PROFILE_SCOPE("update - Window::onUpdate");

				for (auto it = m_layerStack.rbegin(); it != m_layerStack.rend(); ++it)
				{
					(*it)->onUpdate(timeStep, m_data.inputState);
				}
			}

			{
				MN_PROFILE_SCOPE("render - Window::onUpdate");

				for (auto& layer : m_layerStack)
				{
					layer->onRender(*m_renderer2D);
				}
			}
		}
	}

	void Window::onImGuiRender()
	{
		MN_PROFILE_FUNCTION();

		for (auto& layer : m_layerStack)
			layer->onImGuiRender();
	}

}