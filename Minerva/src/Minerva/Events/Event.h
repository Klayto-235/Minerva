#pragma once

#include "Minerva/core.h"

#include <string>
#include <ostream>

namespace Minerva
{

	enum class EventType
	{
		None = 0,
		WindowResize, WindowClose, WindowFocus, WindowMove,
		KeyPress, KeyRelease,
		TextChar,
		MouseButtonPress, MouseButtonRelease, MouseScroll, MouseMove
	};

	// Could implement as enum class, but need operators.
	enum EventCategory
	{
		None = 0,
		EventCategoryWindow			= BIT(0),
		EventCategoryInput			= BIT(1),
		EventCategoryKey			= BIT(2),
		EventCategoryText			= BIT(3),
		EventCategoryMouse			= BIT(4),
		EventCategoryMouseButton	= BIT(5)
	};

#if defined MN_ENABLE_DEBUG_CODE
#define EVENT_CLASS_TYPE(type) static constexpr EventType getStaticType() { return EventType::##type; }\
								EventType getEventType() const override { return getStaticType(); }\
								const char* getName() const override { return #type "Event"; }
#else
#define EVENT_CLASS_TYPE(type) static constexpr EventType getStaticType() { return EventType::##type; }\
								EventType getEventType() const override { return getStaticType(); }
#endif

#define EVENT_CLASS_CATEGORY(category) int getEventCategoryFlags() const override { return category; }

	class  Event
	{
	public:
		virtual EventType getEventType() const = 0;
		virtual int getEventCategoryFlags() const = 0;
#if defined MN_ENABLE_DEBUG_CODE
		virtual const char* getName() const = 0;
		virtual std::string toString() const { return getName(); };
#endif

		bool isInCategory(EventCategory category) const
		{
			return getEventCategoryFlags() & category;
		}
	};

#if defined MN_ENABLE_DEBUG_CODE
	inline std::ostream& operator<<(std::ostream& os, const Event& e)
	{
		return os << e.toString();
	}
#endif

}