#pragma once

#include "mnpch.h"

#include "core.h"
#include "Events/Event.h"


namespace Minerva
{

	struct WindowProperties
	{
		std::string title;
		unsigned int width;
		unsigned int height;

		WindowProperties(const std::string& title = "Minerva Engine", unsigned int width = 1280, unsigned int height = 720)
			: title(title), width(width), height(height) {}
	};

	class MINERVA_API Window
	{
	public:
		virtual ~Window() {}

		virtual void onUpdate() = 0;

		virtual unsigned int getWidth() const = 0;
		virtual unsigned int getHeight() const = 0;
		virtual void setVSync(bool enabled) = 0;
		virtual bool isVSync() const = 0;
		virtual const EventBuffer& getEventBuffer() const = 0;

		// Defined in platform-specific files.
		static inline void init();
		static inline Window* create(const WindowProperties& properties = WindowProperties());
		static inline void pollEvents();
	};

}