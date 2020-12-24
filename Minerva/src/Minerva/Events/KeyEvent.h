#pragma once

#include "Minerva/Events/Event.h"
#include "Minerva/core/input_codes.h"

#include <sstream>


namespace Minerva
{

	class  KeyEvent : public Event
	{
	public:
		Key getKey() const { return m_key; }

		EVENT_CLASS_CATEGORY(EventCategoryKey | EventCategoryInput)
	protected:
		KeyEvent(Key key)
			: m_key(key) {}

		Key m_key;
	};

	class  KeyPressEvent : public KeyEvent
	{
	public:
		KeyPressEvent(Key key, bool repeat)
			: KeyEvent(key), m_repeat(repeat) {}

		int isRepeat() const { return m_repeat; }

#if defined MN_ENABLE_DEBUG_CODE
		std::string toString() const override
		{
			std::stringstream ss;
			ss << "KeyPressEvent: " << static_cast<int>(m_key);
			if (m_repeat) ss << " (repeat)";
			return ss.str();
		}
#endif

		EVENT_CLASS_TYPE(KeyPress)
	private:
		bool m_repeat;
	};

	class  KeyReleaseEvent : public KeyEvent
	{
	public:
		KeyReleaseEvent(Key key)
			: KeyEvent(key) {}

#if defined MN_ENABLE_DEBUG_CODE
		std::string toString() const override
		{
			std::stringstream ss;
			ss << "KeyReleaseEvent: " << static_cast<int>(m_key);
			return ss.str();
		}
#endif

		EVENT_CLASS_TYPE(KeyRelease)
	};

	class  TextCharEvent : public Event
	{
	public:
		TextCharEvent(unsigned int codePoint)
			: m_codePoint(codePoint) {}

		unsigned int getCodePoint() const { return m_codePoint; }

#if defined MN_ENABLE_DEBUG_CODE
		std::string toString() const override
		{
			std::stringstream ss;
			ss << "TextCharEvent: " << m_codePoint;
			return ss.str();
		}
#endif

		EVENT_CLASS_TYPE(TextChar)
		EVENT_CLASS_CATEGORY(EventCategoryText | EventCategoryInput)
	private:
		unsigned int m_codePoint;
	};

}