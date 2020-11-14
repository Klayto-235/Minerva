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
		KeyPressEvent(int keyCode, int repeatCount)
			: KeyEvent(keyCode), m_repeatCount(repeatCount) {}

		int getRepeatCount() const { return m_repeatCount; }

#if defined MN_DEBUG || defined MN_RELEASE
		std::string toString() const override
		{
			std::stringstream ss;
			ss << "KeyPressEvent: " << m_keyCode << " (" << m_repeatCount << " repeats)";
			return ss.str();
		}
#endif

		EVENT_CLASS_TYPE(KeyPress)
	private:
		int m_repeatCount;
	};

	class MINERVA_API KeyReleaseEvent : public KeyEvent
	{
	public:
		KeyReleaseEvent(int keyCode)
			: KeyEvent(keyCode) {}

#if defined MN_DEBUG || defined MN_RELEASE
		std::string toString() const override
		{
			std::stringstream ss;
			ss << "KeyReleaseEvent: " << m_keyCode;
			return ss.str();
		}
#endif

		EVENT_CLASS_TYPE(KeyPress)
	};

}