#pragma once

#include "core.h"
#include "Window.h"

namespace Minerva
{

	class MINERVA_API Application
	{
	public:
		Application();
		virtual ~Application();

		void run();
	private:
		std::unique_ptr<Window> m_window;
		bool m_running;
	};

	// To be defined in CLIENT
	Application* createApplication();

}