#pragma once

#include "Minerva/Events/Event.h"
#include "Minerva/core/Window.h"

#include <sstream>


namespace Minerva
{

	class WindowResizeEvent : public Event
	{
	public:
		WindowResizeEvent(uint32_t width, uint32_t height)
			: m_width(width), m_height(height) {}

		uint32_t getWidth() const { return m_width; }
		uint32_t getHeight() const { return m_height; }

#if defined MN_ENABLE_DEBUG_CODE
		std::string toString() const override
		{
			std::stringstream ss;
			ss << "WindowResizeEvent: " << m_width << " x " << m_height;
			return ss.str();
		};
#endif

		EVENT_CLASS_TYPE(WindowResize)
		EVENT_CLASS_CATEGORY(EventCategoryWindow)
	private:
		uint32_t m_width, m_height;
	};

	class WindowCloseEvent : public Event
	{
	public:
		explicit WindowCloseEvent(Window* window)
			: m_window(window) {}

		Window* getWindow() const { return m_window; }

#if defined MN_ENABLE_DEBUG_CODE
		std::string toString() const override
		{
			std::stringstream ss;
			ss << "WindowCloseEvent: \"" << m_window->getTitle() << '"';
			return ss.str();
		};
#endif

		EVENT_CLASS_TYPE(WindowClose)
		EVENT_CLASS_CATEGORY(EventCategoryWindow)
	private:
		Window* m_window;
	};

}