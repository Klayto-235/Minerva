#pragma once

#include "core.h"
#include "Events/Event.h"

#include <string>


namespace Minerva
{

	class  Layer
	{
	public:
		virtual ~Layer() = default;
		Layer(const Layer& other) = delete;
		Layer& operator=(const Layer& other) = delete;

		virtual void onAttach() {}
		virtual void onDetach() {}
		virtual void onUpdate(float timeStep) {}
		virtual void onImGuiRender() {}
		virtual bool onEvent(const Event& event) { return false; }

#if defined MN_ENABLE_DEBUG_CODE
		const std::string& getName() const { return m_debugName; }
	protected:
		std::string m_debugName;
		explicit Layer(const std::string& name = "Layer")
			: m_debugName(name) {}
#else
	protected:
		explicit Layer(const std::string& name = "Layer") = default;
#endif
		
	};
}