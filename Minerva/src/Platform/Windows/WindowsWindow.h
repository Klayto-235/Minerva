#pragma once

#include "Minerva/core/Window.h"
#include "Minerva/Renderer/GraphicsContext.h"

#include <GLFW/glfw3.h>


namespace Minerva
{
	class WindowsWindow;

	class WindowsWindowInputState : public WindowInputState
	{
	public:
		explicit WindowsWindowInputState(const WindowsWindow& window)
			: m_window(window) {}

		bool isKeyPressed(Key key) const override;
		bool isMouseButtonPressed(MouseButton button) const override;
		std::pair<float, float> getMousePosition() const override;
		float getMouseX() const override;
		float getMouseY() const override;
	private:
		const WindowsWindow& m_window;
	};

	class WindowsWindow : public Window
	{
	public:
		explicit WindowsWindow(const WindowProperties& properties);
		~WindowsWindow();

		void makeContextCurrent() const override { glfwMakeContextCurrent(m_windowHandle); }
		void swapBuffers() override { m_context->swapBuffers(); }

		void setVSync(bool enabled) override;
		void* getNativeWindow() override { return static_cast<void*>(m_windowHandle); }
		const WindowInputState& getInputState() const override { return m_inputState; }
	private:
		friend class WindowsWindowInputState;

		GLFWwindow* m_windowHandle;
		Scope<GraphicsContext> m_context;
		WindowsWindowInputState m_inputState;
	};

}