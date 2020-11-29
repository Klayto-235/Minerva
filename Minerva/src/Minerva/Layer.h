#pragma once

#include "core.h"
#include "Events/Event.h"

#include <string>


namespace Minerva
{

	class  Layer
	{
	public:
		explicit Layer(const std::string& name = "Layer")
#if defined MN_ENABLE_DEBUG_CODE
			: m_debugName(name) 
#endif
		{}
		virtual ~Layer() = default;
		Layer(const Layer& other) = delete;
		Layer& operator=(const Layer& other) = delete;

		virtual void onAttach() {}
		virtual void onDetach() {}
		virtual void onUpdate() {}
		virtual void onImGuiRender() {}
		virtual bool onEvent(const Event& event) { return false; }

#if defined MN_ENABLE_DEBUG_CODE
		inline const std::string& getName() const { return m_debugName; }
	protected:
		std::string m_debugName;
#endif
	};
}