#pragma once

#include "Minerva/core/core.h"
#include "Minerva/Events/EventBuffer.h"
#include "Minerva/core/input_codes.h"
#include "Minerva/core/LayerStack.h"

#include <string>


namespace Minerva
{
#ifdef MN_ENABLE_ASSERTS
	extern bool g_lockWindows;
#endif

	class Application;

	// Can't be a nested class due to a circular dependency.
	class WindowInputState
	{
	public:
		virtual bool isKeyPressed(Key key) const = 0;
		virtual bool isMouseButtonPressed(MouseButton button) const = 0;
		virtual std::pair<float, float> getMousePosition() const = 0;
		virtual float getMouseX() const = 0;
		virtual float getMouseY() const = 0;
	};

	struct WindowProperties
	{
		std::string title;
		unsigned int width;
		unsigned int height;

		WindowProperties(const std::string& title = "Minerva Engine",
			unsigned int width = 1280, unsigned int height = 720)
			: title(title), width(width), height(height) {}
	};

	class Window
	{
	public:
		virtual ~Window() = default;
		Window(const Window & other) = delete;
		Window& operator=(const Window& other) = delete;

		// Basic info
		std::string getTitle() const { return m_data.title; }
		unsigned int getWidth() const { return m_data.width; }
		unsigned int getHeight() const { return m_data.height; }
		bool isVSync() const { return m_data.VSync; }

		// Virtuals
		virtual void setVSync(bool enabled) = 0;
		virtual void* getNativeWindow() = 0;
		virtual const WindowInputState& getInputState() const = 0;

		// Events
		const EventBuffer& getEventBuffer() const { return m_data.eventBuffer; }
		template <typename T = Event, typename ...Args>
		void postEvent(Args&&... args) { m_data.eventBuffer.add<T, Args>(std::forward<Args>(args)...); }

		// Owned Layer(s)
		template<typename T, typename ...Args>
		T* createLayer(Args&&... args);
		void deleteLayer(Layer* layer);
		void deleteLayers();

		// LayerStack
		void pushLayer(Layer* layer);
		void pushOverlay(Layer* overlay);
		Layer* popLayer();
		Layer* popOverlay();
		int removeLayer(Layer* layer);
		int removeOverlay(Layer* overlay);
		void clearLayerStack();

	protected:
		Window(const WindowProperties& properties)
			: m_data{ properties.title, properties.width, properties.height, false } {}

		struct WindowData
		{
			std::string title;
			unsigned int width;
			unsigned int height;
			bool VSync;
			EventBuffer eventBuffer;
		};
		WindowData m_data;
	private:
		friend class Application;

		virtual void makeContextCurrent() const = 0;
		virtual void swapBuffers() = 0;

		void onUpdate(const float timeStep, EventBuffer& appEventBuffer);
		void onImGuiRender();

		// Defined in platform-specific files.
		static void init();
		static void terminate();
		static void pollEvents();
		static Scope<Window> create(const WindowProperties& properties);

		bool m_minimised = false;
		std::vector<Scope<Layer>> m_layers;
		LayerStack m_layerStack;
	};

	template<typename T, typename ...Args>
	inline T* Window::createLayer(Args && ...args)
	{
		static_assert(std::is_base_of<Layer, T>::value, "Window::createLayer: Invalid template argument.");
		MN_CORE_ASSERT(!g_lockWindows, "Window::createLayer: Cannot create layer while windows are locked.");
		makeContextCurrent();
		m_layers.push_back(createScope<T>(std::forward<Args>(args)...));
#if defined MN_ENABLE_ASSERTS
		m_layers.back()->m_owner = this;
#endif
		return static_cast<T*>(m_layers.back().get());
	}

	inline void Window::deleteLayer(Layer* layer)
	{
		MN_CORE_ASSERT(!g_lockWindows, "Window::deleteLayer: Cannot delete layer while windows are locked.");
		auto it = std::find_if(m_layers.begin(), m_layers.end(),
			[layer](const Scope<Layer>& other) { return layer == other.get(); });
		MN_CORE_ASSERT(it != m_layers.end(), "Window::deleteLayer: Could not find layer.");
		makeContextCurrent();
		removeLayer(layer);
		m_layers.erase(it);
	}

	inline void Window::deleteLayers()
	{
		MN_CORE_ASSERT(!g_lockWindows, "Window::deleteLayers: Cannot delete layers while windows are locked.");
		makeContextCurrent();
		for (auto& layer : m_layers)
		{
			m_layerStack.removeOverlay(layer.get());
			m_layerStack.removeLayer(layer.get());
		}
		m_layers.clear();
	}

	inline void Window::pushLayer(Layer* layer)
	{
		MN_CORE_ASSERT(!g_lockWindows, "Window::pushLayer: Cannot push layer while windows are locked.");
		MN_CORE_ASSERT(layer->m_owner == nullptr || layer->m_owner == this,
			"Window::pushLayer: Cannot push layer that is owned by another window.");
		m_layerStack.pushLayer(layer);
	}

	inline void Window::pushOverlay(Layer* overlay)
	{
		MN_CORE_ASSERT(!g_lockWindows, "Window::pushOverlay: Cannot push overlay while windows are locked.");
		MN_CORE_ASSERT(overlay->m_owner == nullptr || overlay->m_owner == this,
			"Window::pushOverlay: Cannot push overlay that is owned by another window.");
		m_layerStack.pushOverlay(overlay);
	}

	inline Layer* Window::popLayer()
	{
		MN_CORE_ASSERT(!g_lockWindows, "Window::popLayer: Cannot pop layer while windows are locked.");
		return m_layerStack.popLayer();
	}

	inline Layer* Window::popOverlay()
	{
		MN_CORE_ASSERT(!g_lockWindows, "Window::popOverlay: Cannot pop overlay while windows are locked.");
		return m_layerStack.popOverlay();
	}

	inline int Window::removeLayer(Layer* layer)
	{
		MN_CORE_ASSERT(!g_lockWindows, "Window::removeLayer: Cannot remove layer while windows are locked.");
		return m_layerStack.removeLayer(layer);
	}

	inline int Window::removeOverlay(Layer* overlay)
	{
		MN_CORE_ASSERT(!g_lockWindows, "Window::removeOverlay: Cannot remove overlay while windows are locked.");
		return m_layerStack.removeOverlay(overlay);
	}

	inline void Window::clearLayerStack()
	{
		MN_CORE_ASSERT(!g_lockWindows, "Window::clearLayerStack: Cannot clear layer stack while windows are locked.");
		m_layerStack.clear();
	}
}