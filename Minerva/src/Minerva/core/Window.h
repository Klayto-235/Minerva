#pragma once

#include "Minerva/core/core.h"
#include "Minerva/Events/EventBuffer.h"
#include "Minerva/core/input_codes.h"
#include "Minerva/Renderer/Renderer2D.h"
#include "Minerva/core/LayerStack.h"
#include "Minerva/debug/Profiler.h"

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
		uint32_t width;
		uint32_t height;

		WindowProperties(const std::string& title = "Minerva Engine",
			uint32_t width = 1280, uint32_t height = 720)
			: title(title), width(width), height(height) {}
	};
	
	/** The base abstract class for platform-specific window implementations.
	 * 
	 * This class contains all platform-independent window functionality and exposes platform-dependent
	 * features through virtual functions. It provides access to internal @ref WindowInputState,
	 * @ref EventBuffer, and @ref LayerStack fields. Additionally, it allows the user to create
	 * @ref Layer objects owned by the window. This ensures that the layers are destroyed with the
	 * same graphics context bound as when they were created.
	 * 
	 * @ref Event objects can be added to the internal event buffer by calling @postEvent. Events are
	 * processed and cleared each frame when the window is updated.
	 * 
	 * Generally, layers can be pushed multiple times and to different windows. However, layers with
	 * OpenGL resources cannot be shared between windows and should be created with @ref createLayer.
	 * Owned layer migration between windows is checked when asserts are enabled.
	 * 
	 * Owned layer and layer stack manipulation functions cannot be called from layers, but may be
	 * called from the client application class.
	*/
	class Window
	{
	public:
		virtual ~Window() = default;
		Window(const Window & other) = delete;
		Window& operator=(const Window& other) = delete;

		// Basic info
		std::string getTitle() const { return m_data.title; }
		uint32_t getWidth() const { return m_data.width; }
		uint32_t getHeight() const { return m_data.height; }
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
		/// Creates an owned layer and returns a pointer to it.
		template<typename T, typename ...Args>
		T* createLayer(Args&&... args);
		/// Removes an owned layer from the layer stack and deletes it.
		void deleteLayer(Layer* layer);
		/// Removes all owned layers from the layer stack and deletes them.
		void deleteLayers();

		// LayerStack
		void pushLayer(Layer* layer);
		void pushOverlay(Layer* overlay);
		Layer* popLayer();
		Layer* popOverlay();
		size_t removeLayer(Layer* layer);
		size_t removeOverlay(Layer* overlay);
		void clearLayerStack();

	protected:
		Window(const WindowProperties& properties)
			: m_data{ properties.title, properties.width, properties.height, false } {}

		void initResources();
		void freeResources();

		struct WindowData
		{
			std::string title;
			uint32_t width;
			uint32_t height;
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
		Scope<Renderer2D> m_renderer2D;
		std::vector<Scope<Layer>> m_layers;
		LayerStack m_layerStack;
	};

	template<typename T, typename ...Args>
	inline T* Window::createLayer(Args && ...args)
	{
		MN_PROFILE_FUNCTION();

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
		MN_PROFILE_FUNCTION();

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
		MN_PROFILE_FUNCTION();

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
		MN_PROFILE_FUNCTION();

		MN_CORE_ASSERT(!g_lockWindows, "Window::pushLayer: Cannot push layer while windows are locked.");
		MN_CORE_ASSERT(layer->m_owner == nullptr || layer->m_owner == this,
			"Window::pushLayer: Cannot push layer that is owned by another window.");
		m_layerStack.pushLayer(layer);
	}

	inline void Window::pushOverlay(Layer* overlay)
	{
		MN_PROFILE_FUNCTION();

		MN_CORE_ASSERT(!g_lockWindows, "Window::pushOverlay: Cannot push overlay while windows are locked.");
		MN_CORE_ASSERT(overlay->m_owner == nullptr || overlay->m_owner == this,
			"Window::pushOverlay: Cannot push overlay that is owned by another window.");
		m_layerStack.pushOverlay(overlay);
	}

	inline Layer* Window::popLayer()
	{
		MN_PROFILE_FUNCTION();

		MN_CORE_ASSERT(!g_lockWindows, "Window::popLayer: Cannot pop layer while windows are locked.");
		return m_layerStack.popLayer();
	}

	inline Layer* Window::popOverlay()
	{
		MN_PROFILE_FUNCTION();

		MN_CORE_ASSERT(!g_lockWindows, "Window::popOverlay: Cannot pop overlay while windows are locked.");
		return m_layerStack.popOverlay();
	}

	inline size_t Window::removeLayer(Layer* layer)
	{
		MN_PROFILE_FUNCTION();

		MN_CORE_ASSERT(!g_lockWindows, "Window::removeLayer: Cannot remove layer while windows are locked.");
		return m_layerStack.removeLayer(layer);
	}

	inline size_t Window::removeOverlay(Layer* overlay)
	{
		MN_PROFILE_FUNCTION();

		MN_CORE_ASSERT(!g_lockWindows, "Window::removeOverlay: Cannot remove overlay while windows are locked.");
		return m_layerStack.removeOverlay(overlay);
	}

	inline void Window::clearLayerStack()
	{
		MN_PROFILE_FUNCTION();

		MN_CORE_ASSERT(!g_lockWindows, "Window::clearLayerStack: Cannot clear layer stack while windows are locked.");
		m_layerStack.clear();
	}
}