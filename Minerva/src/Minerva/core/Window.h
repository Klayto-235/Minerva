#pragma once

#include "Minerva/core/core.h"
#include "Minerva/Events/EventBuffer.h"
#include "Minerva/core/input_codes.h"
#include "Minerva/Renderer/Renderer2D.h"
#include "Minerva/core/LayerStack.h"
#include "Minerva/debug/Profiler.h"
#include "Minerva/Renderer/GraphicsContext.h"

#include <string>
#include <bitset>


namespace Minerva
{
#ifdef MN_ENABLE_ASSERTS
	extern bool g_lockWindows;
#endif

	class Application;

	// Can't be a nested class in Window due to a circular dependency.
	class WindowInputState
	{
	public:
		bool isKeyPressed(Key key) const { return m_keyState[(int)key]; }
		bool isMouseButtonPressed(MouseButton button) const { return m_mouseButtonState[(int)button]; }
		std::pair<float, float> getMousePosition() const { return m_mousePosition; }
		float getMouseX() const { return m_mousePosition.first; }
		float getMouseY() const { return m_mousePosition.second; }

		// Should be private but GLFW callbacks are lambdas
		void setKey(Key key, bool pressed) { m_keyState[(int)key] = pressed; }
		void setMouseButton(MouseButton button, bool pressed) { m_mouseButtonState[(int)button] = pressed; }
		void setMousePosition(float x, float y) { m_mousePosition = { x, y }; }
		void onLoseFocus() { m_keyState.reset(); m_mouseButtonState.reset(); }
	private:
		std::bitset<(int)Key::SIZE> m_keyState;
		std::bitset<(int)MouseButton::SIZE> m_mouseButtonState;
		std::pair<float, float> m_mousePosition = { 0.0f, 0.0f };
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
	
	/** This class represents and implements an application window.
	 * 
	 * The implementation of this class is split into a platform-dependent and platform-independent
	 * part. It provides access to internal @ref WindowInputState,
	 * @ref EventBuffer, and @ref LayerStack fields. Additionally, it allows the user to create
	 * @ref Layer objects owned by the window. This ensures that the layers are destroyed with the
	 * same graphics context bound as when they were created.
	 * 
	 * @ref Event objects can be added to the internal event buffer by calling @ref postEvent. Events
	 * are processed and cleared each frame when the window is updated.
	 * 
	 * Generally, layers can be pushed multiple times and to different windows. However, layers with
	 * OpenGL resources cannot be shared between windows and should be created with @ref newLayer.
	 * Owned layer migration between windows is checked when asserts are enabled.
	 * 
	 * Owned layer and layer stack manipulation functions cannot be called from layers, but may be
	 * called from the client application class.
	*/
	class Window
	{
	public:
		~Window();
		Window(const Window & other) = delete;
		Window& operator=(const Window& other) = delete;

		// Basic info
		std::string getTitle() const { return m_data.title; }
		uint32_t getWidth() const { return m_data.width; }
		uint32_t getHeight() const { return m_data.height; }
		bool isVSync() const { return m_VSync; }
		void setVSync(bool enable);
		void* getNativeWindow() const { return m_nativeWindowHandle; }

		// Events
		template <typename T = Event, typename ...Args>
		void postEvent(Args&&... args) { m_data.eventBuffer.add<T, Args>(std::forward<Args>(args)...); }

		// Owned Layer(s)
		/// Creates an owned layer and returns a pointer to it.
		template<typename T, typename ...Args>
		T* newLayer(Args&&... args);
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

	private:
		friend class Application;

		Window(const WindowProperties& properties);

		void impl_createWindow();
		void impl_destroyWindow();

		void onUpdate(const float timeStep, EventBuffer& appEventBuffer);
		void onImGuiRender();

		static void init();
		static void terminate();
		static void pollEvents();

		struct WindowData
		{
			std::string title;
			uint32_t width;
			uint32_t height;
			EventBuffer eventBuffer;
			WindowInputState inputState;
		};
		WindowData m_data;

		void* m_nativeWindowHandle = nullptr;
		Scope<GraphicsContext> m_graphicsContext;
		bool m_VSync = false;
		bool m_minimised = false;
		std::vector<Scope<Layer>> m_layers;
		LayerStack m_layerStack;
		Scope<Renderer2D> m_renderer2D;
	};

	template<typename T, typename ...Args>
	T* Window::newLayer(Args && ...args)
	{
		MN_PROFILE_FUNCTION();

		static_assert(std::is_base_of<Layer, T>::value, "Window::newLayer: Invalid template argument.");
		MN_CORE_ASSERT(!g_lockWindows, "Window::newLayer: Cannot create layer while windows are locked.");
		m_graphicsContext->makeCurrent();
		m_layers.push_back(createScope<T>(std::forward<Args>(args)...));
#if defined MN_ENABLE_ASSERTS
		m_layers.back()->m_owner = this;
#endif
		return static_cast<T*>(m_layers.back().get());
	}

}