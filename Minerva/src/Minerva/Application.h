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
	private:

		std::unique_ptr<Window> m_window;
		bool m_running;
		LayerStack m_layerStack;
	};

	// To be defined in CLIENT
	Application* createApplication();

}