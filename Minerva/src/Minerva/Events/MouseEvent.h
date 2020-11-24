#pragma once

#include "Event.h"

namespace Minerva
{

	class MINERVA_API MouseButtonEvent : public Event
	{
	public:
		int getMouseButton() const { return m_button; }

		EVENT_CLASS_CATEGORY(EventCategoryMouseButton | EventCategoryMouse | EventCategoryInput)
	protected:
		MouseButtonEvent(int button)
			: m_button(button) {}

		int m_button;
	};

	class MINERVA_API MouseButtonPressEvent : public MouseButtonEvent
	{
	public:
		MouseButtonPressEvent(int button)
			: MouseButtonEvent(button) {}

#if defined MN_ENABLE_DEBUG_CODE
		std::string toString() const override
		{
			std::stringstream ss;
			ss << "MouseButtonPressEvent: " << m_button;
			return ss.str();
		}
#endif

		EVENT_CLASS_TYPE(MouseButtonPress)
	};

	class MINERVA_API MouseButtonReleaseEvent : public MouseButtonEvent
	{
	public:
		MouseButtonReleaseEvent(int button)
			: MouseButtonEvent(button) {}

#if defined MN_ENABLE_DEBUG_CODE
		std::string toString() const override
		{
			std::stringstream ss;
			ss << "MouseButtonReleaseEvent: " << m_button;
			return ss.str();
		}
#endif

		EVENT_CLASS_TYPE(MouseButtonRelease)
	};

	class MINERVA_API MouseMoveEvent : public Event
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

	class MINERVA_API MouseScrollEvent : public Event
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