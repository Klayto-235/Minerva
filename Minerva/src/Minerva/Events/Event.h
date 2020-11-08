#pragma once

#include "Minerva/core.h"

#include <string>

namespace Minerva {

	enum class EventType
	{
		None = 0,
		WindowResize, WindowClose, WindowFocus, WindowMove,
		KeyPress, KeyRelease,
		MouseButtonPress, MouseButtonRelease, MouseScroll, MouseMove
	};

	// Could implement as enum class, but need operators.
	enum EventCategory
	{
		None = 0,
		EventCategoryWindow		 = BIT(0),
		EventCategoryInput		 = BIT(1),
		EventCategoryKey		 = BIT(2),
		EventCategoryMouse		 = BIT(3),
		EventCategoryMouseButton = BIT(4)
	};

#if defined MN_DEBUG || defined MN_RELEASE
#define EVENT_CLASS_TYPE(type) static constexpr EventType GetStaticType() { return EventType::##type; }\
								virtual EventType GetEventType() const override { return GetStaticType(); }\
								virtual const char* GetName() const override { return #type "Event"; }
#else
#define EVENT_CLASS_TYPE(type) static constexpr EventType GetStaticType() { return EventType::##type; }\
								virtual EventType GetEventType() const override { return GetStaticType(); }
#endif

#define EVENT_CLASS_CATEGORY(category) virtual int GetEventCategoryMask() const override { return category; }

	class MINERVA_API Event
	{
	public:
		virtual EventType GetEventType() const = 0;
		virtual int GetEventCategoryMask() const = 0;
#if defined MN_DEBUG || defined MN_RELEASE
		virtual const char* GetName() const = 0;
		virtual std::string ToString() const { return GetName(); };
#endif

		inline bool IsInCategory(EventCategory category)
		{
			return GetEventCategoryMask() & category;
		}
	};

#if defined MN_DEBUG || defined MN_RELEASE
	inline std::ostream& operator<<(std::ostream& os, const Event& e)
	{
		return os << e.ToString();
	}
#endif

}