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
		Application();
		virtual ~Application();

		void run();

		void pushLayer(Layer* layer);
		void pushOverlay(Layer* overlay);

		Window& getWindow() { return *m_window; }

		static Application& get() { return *s_instance; }
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