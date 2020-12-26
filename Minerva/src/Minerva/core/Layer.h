#pragma once

#include "Minerva/core/core.h"
#include "Minerva/Events/Event.h"
#include "Minerva/Renderer/Renderer2D.h"

#include <string>


namespace Minerva
{
#if defined MN_ENABLE_ASSERTS
	class Window;
#endif

	class WindowInputState;

	class Layer
	{
	public:
		virtual ~Layer() = default;
		Layer(const Layer& other) = delete;
		Layer& operator=(const Layer& other) = delete;

		virtual void onAttach() {}
		virtual void onDetach() {}
		virtual bool onEvent(const Event& event) { return false; }
		virtual void onUpdate(const float timeStep, const WindowInputState& inputState) {}
		virtual void onRender(Minerva::Renderer2D& renderer2D) {};
		virtual void onImGuiRender() {}

#if defined MN_ENABLE_DEBUG_CODE
		const std::string& getName() const { return m_name; }
	protected:
		std::string m_name;
		explicit Layer(const std::string& name = "Layer")
			: m_name(name) {}
#else
	protected:
		explicit Layer(const std::string& name = "Layer") {};
#endif

#if defined MN_ENABLE_ASSERTS
	private:
		friend class Window;

		const Window* m_owner = nullptr;
#endif
	};

}