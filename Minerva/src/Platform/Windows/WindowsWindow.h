#pragma once

#include "Minerva/core/Window.h"
#include "Minerva/Renderer/GraphicsContext.h"

#include <GLFW/glfw3.h>


namespace Minerva
{
	class WindowsWindow : public Window
	{
	public:
		class WindowsInputState : public InputState
		{
		public:
			explicit WindowsInputState(const WindowsWindow& window)
				: m_window(window) {}

			bool isKeyPressed(Key key) const override;
			bool isMouseButtonPressed(MouseButton button) const override;
			std::pair<float, float> getMousePosition() const override;
			float getMouseX() const override;
			float getMouseY() const override;
		private:
			const WindowsWindow& m_window;
		};

		explicit WindowsWindow(const WindowProperties& properties);
		~WindowsWindow();

		void onUpdate() override;

		unsigned int getWidth() const override { return m_data.width; }
		unsigned int getHeight() const override { return m_data.height; }
		void setVSync(bool enabled) override;
		bool isVSync() const override { return m_data.VSync; }
		void* getNativeWindow() override { return static_cast<void*>(m_window); }
		const EventBuffer& getEventBuffer() const override { return m_data.eventBuffer; }
		const InputState& getInputState() const override { return m_inputState; }
	private:
		GLFWwindow* m_window;
		Scope<GraphicsContext> m_context;

		struct WindowData
		{
			std::string title;
			unsigned int width;
			unsigned int height;
			bool VSync;
			EventBuffer eventBuffer;
		};

		WindowData m_data;
		WindowsInputState m_inputState;
	};

}