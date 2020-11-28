#pragma once

#include "core.h"
#include "Events/Event.h"

#include <string>


namespace Minerva
{

	class MINERVA_API Layer
	{
	public:
		Layer(const std::string& name = "Layer")
#if defined MN_ENABLE_DEBUG_CODE
			: m_debugName(name) 
#endif
		{}
		virtual ~Layer() {}

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