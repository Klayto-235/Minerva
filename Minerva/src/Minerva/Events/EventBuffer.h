#pragma once

#include "Event.h"

#include <vector>
#include <type_traits>
#include <memory>
#include <algorithm>

namespace Minerva
{

	class EventBuffer
	{
	public:
		class Iterator;

		EventBuffer()
		{
			m_buffer.reserve(8);
		}
		EventBuffer(const EventBuffer& other) = delete;
		EventBuffer& operator=(const EventBuffer& other) = delete;

		template <class T = Event, class... Args>
		void post(Args&&... args)
		{
			static_assert(std::is_base_of<Event, T>::value, "Invalid template argument for EventBuffer::post.");
			// emplace_back unnecessary since there is a temporary anyway...
			m_buffer.emplace_back(new T(args...));
		}
		void clear()
		{
			std::for_each(m_buffer.begin(), m_buffer.end(), std::default_delete<const Event>());
			m_buffer.clear();
		}

		std::vector<const Event*>::const_iterator begin() const { return m_buffer.begin(); }
		std::vector<const Event*>::const_iterator end() const { return m_buffer.end(); }
	private:
		std::vector<const Event*> m_buffer;
	};

#if defined MN_ENABLE_DEBUG_CODE
	inline std::ostream& operator<<(std::ostream& os, const EventBuffer& ebuf)
	{
		os << "EventBuffer " << &ebuf << ':';
		for (auto& e : ebuf)
		{
			os << "\n\t" << e->toString();
		}
		return os;
	}
#endif

}