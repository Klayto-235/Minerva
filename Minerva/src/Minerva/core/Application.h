#pragma once

#include "Minerva/core/core.h"
#include "Minerva/core/Window.h"
#include "Minerva/ImGui/ImGuiContext.h"
#include "Minerva/Events/EventBuffer.h"

#include <vector>


namespace Minerva
{
	/** The base application class that the client inherits from.
	 *
	 * The glue that holds it all together.
	 * 
	 * Windows are managed by the Application class to ensure correct asset deletion order.
	 * 
	 * Window and ImGui related functions cannot be called from @ref Layer objects, but may be
	 * called from the client application class constructor and overrides of functions such as
	 * Application::onUpdate and Application::onEvent.
	*/
	class Application
	{
	public:
		virtual ~Application();
		Application(const Application& other) = delete;
		Application& operator=(const Application& other) = delete;

		void run();

		/// Creates a window and returns a pointer to it.
		Window* createWindow(const WindowProperties& properties = WindowProperties());
		/// Deletes a window and disables ImGui if it was enabled with this window.
		void deleteWindow(Window* window);

		/// Enables or re-enables ImGui.
		void enableImGui(Window* window);
		/// Disables ImGui if enabled.
		void disableImGui();

		virtual void onEvent(const Event& event);
		virtual void onUpdate() {};

		static Application& get() { return *s_instance; }
	protected:
		Application();
	private:
		static Application* s_instance;

		std::vector<Scope<Window>> m_windows;
		EventBuffer m_eventBuffer;
		bool m_running = false;
		float m_lastFrameTime = 0.0f;
		bool m_enableImGui = false;
		Scope<ImGuiContext> m_ImGuiContext;
		Window* m_ImGuiWindow = nullptr;
	};

	// To be defined in client code.
	Application* createApplication();

}