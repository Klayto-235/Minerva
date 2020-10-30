#pragma once

#include "core.h"

namespace Minerva
{

	class MINERVA_API Application
	{
	public:
		Application();
		virtual ~Application();

		void run();
	};

	// To be defined in CLIENT
	Application* CreateApplication();

}