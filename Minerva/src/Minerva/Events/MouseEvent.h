#pragma once

#include "Minerva/Events/Event.h"
#include "Minerva/core/input_codes.h"

#include <sstream>


namespace Minerva
{

	class MouseButtonEvent : public Event
	{
	public:
		MouseButton getMouseButton() const { return m_button; }

		EVENT_CLASS_CATEGORY(EventCategoryMouseButton | EventCategoryMouse | EventCategoryInput)
	protected:
		MouseButtonEvent(MouseButton button)
			: m_button(button) {}

		MouseButton m_button;
	};

	class MouseButtonPressEvent : public MouseButtonEvent
	{
	public:
		explicit MouseButtonPressEvent(MouseButton button)
			: MouseButtonEvent(button) {}

#if defined MN_ENABLE_DEBUG_CODE
		std::string toString() const override
		{
			std::stringstream ss;
			ss << "MouseButtonPressEvent: " << static_cast<int>(m_button);
			return ss.str();
		}
#endif

		EVENT_CLASS_TYPE(MouseButtonPress)
	};

	class MouseButtonReleaseEvent : public MouseButtonEvent
	{
	public:
		explicit MouseButtonReleaseEvent(MouseButton button)
			: MouseButtonEvent(button) {}

#if defined MN_ENABLE_DEBUG_CODE
		std::string toString() const override
		{
			std::stringstream ss;
			ss << "MouseButtonReleaseEvent: " << static_cast<int>(m_button);
			return ss.str();
		}
#endif

		EVENT_CLASS_TYPE(MouseButtonRelease)
	};

	class MouseMoveEvent : public Event
	{
	public:
		MouseMoveEvent(float x, float y)
			: m_x(x), m_y(y) {}

		float getX() const { return m_x; }
		float getY() const { return m_y; }

#if defined MN_ENABLE_DEBUG_CODE
		std::string toString() const override
		{
			std::stringstream ss;
			ss << "MouseMoveEvent: " << m_x << ", " << m_y;
			return ss.str();
		}
#endif

		EVENT_CLASS_TYPE(MouseMove)
			EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
	private:
		float m_x, m_y;
	};

	class MouseScrollEvent : public Event
	{
	public:
		MouseScrollEvent(float xOffset, float yOffset)
			: m_xOffset(xOffset), m_yOffset(yOffset) {}

		float getXOffset() const { return m_xOffset; }
		float getYOffset() const { return m_yOffset; }

#if defined MN_ENABLE_DEBUG_CODE
		std::string toString() const override
		{
			std::stringstream ss;
			ss << "MouseScrollEvent: " << m_xOffset << ", " << m_yOffset;
			return ss.str();
		}
#endif

		EVENT_CLASS_TYPE(MouseScroll)
		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
	private:
		float m_xOffset, m_yOffset;
	};

}