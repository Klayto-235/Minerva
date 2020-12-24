#pragma once

#include "Minerva/core/core.h"
#include "Minerva/core/Window.h"
#include "Minerva/ImGui/ImGuiContext.h"
#include "Minerva/Events/EventBuffer.h"

#include <vector>


namespace Minerva
{

	class  Application
	{
	public:
		virtual ~Application();
		Application(const Application& other) = delete;
		Application& operator=(const Application& other) = delete;

		void run();

		Window* createWindow(const WindowProperties& properties = WindowProperties());
		void deleteWindow(Window* window);

		void enableImGui(Window* window);
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