#pragma once

#include "Minerva/Events/Event.h"

#include <vector>
#include <type_traits>
#include <algorithm>

namespace Minerva
{

	class EventBuffer
	{
	public:
		class Iterator;

		EventBuffer() = default;
		EventBuffer(const EventBuffer& other) = delete;
		EventBuffer& operator=(const EventBuffer& other) = delete;
		~EventBuffer() { clear(); }

		template <typename T = Event, typename ...Args>
		void add(Args&&... args)
		{
			static_assert(std::is_base_of<Event, T>::value,
				"Invalid template argument: event class must inherit from Event.");
			m_buffer.push_back(new T(std::forward<Args>(args)...));
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