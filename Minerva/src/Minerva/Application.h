#pragma once

#include "core.h"
#include "Window.h"
#include "LayerStack.h"
#include "ImGui/ImGuiLayer.h"

namespace Minerva
{

	class  Application
	{
	public:
		virtual ~Application();
		Application(const Application& other) = delete;
		Application& operator=(const Application& other) = delete;

		void run();

		void pushLayer(Layer* layer);
		void pushOverlay(Layer* overlay);

		Window& getWindow() { return *m_window; }

		static Application& get() { return *s_instance; }
	protected:
		Application();
	private:
		std::unique_ptr<Window> m_window;
		bool m_running;
		LayerStack m_layerStack;
		ImGuiLayer* m_ImGuiLayer;

		static Application* s_instance;
	};

	// To be defined in CLIENT
	Application* createApplication();

}