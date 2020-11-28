#pragma once

#include "core.h"
#include "Events/EventBuffer.h"
#include "input_codes.h"

#include <string>


namespace Minerva
{

	struct WindowProperties
	{
		std::string title;
		unsigned int width;
		unsigned int height;

		WindowProperties(const std::string& title = "Minerva Engine",
			unsigned int width = 1280, unsigned int height = 720)
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

		virtual bool isKeyPressed(Key key) const = 0;
		virtual bool isMouseButtonPressed(MouseButton button) const = 0;
		virtual std::pair<float,float> getMousePosition() const = 0;
		virtual float getMouseX() const = 0;
		virtual float getMouseY() const = 0;

		// Defined in platform-specific files.
		static void init();
		static Window* create(const WindowProperties& properties = WindowProperties());
		static void pollEvents();
	};

}