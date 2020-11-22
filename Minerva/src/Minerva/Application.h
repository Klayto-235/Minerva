#pragma once

#include "core.h"
#include "Window.h"
#include "LayerStack.h"

namespace Minerva
{

	class MINERVA_API Application
	{
	public:
		Application();
		virtual ~Application();

		void run();

		void pushLayer(Layer* layer);
		void pushOverlay(Layer* overlay);

		const Window& getWindow() const { return *m_window; }

		static const Application& get() { return *s_instance; }
	private:
		std::unique_ptr<Window> m_window;
		bool m_running;
		LayerStack m_layerStack;

		static Application* s_instance;
	};

	// To be defined in CLIENT
	Application* createApplication();

}