#pragma once

#include "Event.h"

namespace Minerva
{

	class MINERVA_API KeyEvent : public Event
	{
	public:
		int getKeyCode() const { return m_keyCode; }

		EVENT_CLASS_CATEGORY(EventCategoryKey | EventCategoryInput)
	protected:
		KeyEvent(int keyCode)
			: m_keyCode(keyCode) {}

		int m_keyCode;
	};

	class MINERVA_API KeyPressEvent : public KeyEvent
	{
	public:
		KeyPressEvent(int keyCode, bool repeat)
			: KeyEvent(keyCode), m_repeat(repeat) {}

		int isRepeat() const { return m_repeat; }

#if defined MN_ENABLE_DEBUG_CODE
		std::string toString() const override
		{
			std::stringstream ss;
			ss << "KeyPressEvent: " << m_keyCode;
			if (m_repeat) ss << " (repeat)";
			return ss.str();
		}
#endif

		EVENT_CLASS_TYPE(KeyPress)
	private:
		bool m_repeat;
	};

	class MINERVA_API KeyReleaseEvent : public KeyEvent
	{
	public:
		KeyReleaseEvent(int keyCode)
			: KeyEvent(keyCode) {}

#if defined MN_ENABLE_DEBUG_CODE
		std::string toString() const override
		{
			std::stringstream ss;
			ss << "KeyReleaseEvent: " << m_keyCode;
			return ss.str();
		}
#endif

		EVENT_CLASS_TYPE(KeyRelease)
	};

	class MINERVA_API KeyCharEvent : public KeyEvent
	{
	public:
		KeyCharEvent(int keyCode)
			: KeyEvent(keyCode) {}

#if defined MN_ENABLE_DEBUG_CODE
		std::string toString() const override
		{
			std::stringstream ss;
			ss << "KeyCharEvent: " << m_keyCode;
			return ss.str();
		}
#endif

		EVENT_CLASS_TYPE(KeyChar)
	};

}