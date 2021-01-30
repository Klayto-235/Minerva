#pragma once

#include "Minerva/core/input_codes.h"

#include <bitset>
#include <utility>


namespace Minerva
{

	class InputState
	{
	public:
		bool isKeyPressed(Key key) const { return m_keyState[(int)key]; }
		bool isMouseButtonPressed(MouseButton button) const { return m_mouseButtonState[(int)button]; }
		std::pair<float, float> getMousePosition() const { return m_mousePosition; }
		float getMouseX() const { return m_mousePosition.first; }
		float getMouseY() const { return m_mousePosition.second; }

		// Should be private but GLFW callbacks are lambdas
		void setKey(Key key, bool pressed) { m_keyState[(int)key] = pressed; }
		void setMouseButton(MouseButton button, bool pressed) { m_mouseButtonState[(int)button] = pressed; }
		void setMousePosition(float x, float y) { m_mousePosition = { x, y }; }
		void onLoseFocus() { m_keyState.reset(); m_mouseButtonState.reset(); }
	private:
		std::bitset<(int)Key::SIZE> m_keyState;
		std::bitset<(int)MouseButton::SIZE> m_mouseButtonState;
		std::pair<float, float> m_mousePosition = { 0.0f, 0.0f };
	};

}