#pragma once

#include "mnpch.h" // Is this fine?
#include "Minerva/core.h"

#include <type_traits>

namespace Minerva
{

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
#define EVENT_CLASS_TYPE(type) static constexpr EventType getStaticType() { return EventType::##type; }\
								virtual EventType getEventType() const override { return getStaticType(); }\
								virtual const char* getName() const override { return #type "Event"; }
#else
#define EVENT_CLASS_TYPE(type) static constexpr EventType getStaticType() { return EventType::##type; }\
								virtual EventType getEventType() const override { return getStaticType(); }
#endif

#define EVENT_CLASS_CATEGORY(category) virtual int getEventCategoryMask() const override { return category; }

	class MINERVA_API Event
	{
	public:
		virtual EventType getEventType() const = 0;
		virtual int getEventCategoryMask() const = 0;
#if defined MN_DEBUG || defined MN_RELEASE
		virtual const char* getName() const = 0;
		virtual std::string toString() const { return getName(); };
#endif

		bool isInCategory(EventCategory category) const
		{
			return getEventCategoryMask() & category;
		}
	};

	class EventBuffer
	{
	public:
		class Iterator;

		EventBuffer()
		{
			m_buffer.reserve(8);
		}

		template <class T = Event, class... Args>
		void post(Args&&... args)
		{
			static_assert(std::is_base_of<Event, T>::value, "Invalid template argument for EventBuffer::post.");
			// emplace_back unnecessary? since there is a temporary anyway...
			m_buffer.emplace_back(std::make_unique<const T>(std::forward<Args>(args)...));
		}
		void clear() { m_buffer.clear(); }
		Iterator begin() const { return Iterator(m_buffer.begin()); }
		Iterator end() const { return Iterator(m_buffer.end()); }

		class Iterator
		{
		public:
			Iterator& operator++() { ++m_it; return *this; }
			const Event& operator*() const { return **m_it; }
			bool operator!=(const Iterator& other) const { return m_it != other.m_it; }
		private:
			friend Iterator EventBuffer::begin() const;
			friend Iterator EventBuffer::end() const;

			explicit Iterator(std::vector<std::unique_ptr<const Event>>::const_iterator&& it)
				: m_it(it) {}

			std::vector<std::unique_ptr<const Event>>::const_iterator m_it;
		};
	private:
		std::vector<std::unique_ptr<const Event>> m_buffer;
	};

#if defined MN_DEBUG || defined MN_RELEASE
	inline std::ostream& operator<<(std::ostream& os, const Event& e)
	{
		return os << e.toString();
	}

	inline std::ostream& operator<<(std::ostream& os, const EventBuffer& ebuf)
	{
		os << "EventBuffer " << &ebuf << ':';
		for (auto& e : ebuf)
		{
			os << "\n\t" << e.toString();
		}
		return os;
	}
#endif

}