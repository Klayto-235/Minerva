#pragma once

#include "Minerva/Window.h"

#include <GLFW/glfw3.h>


namespace Minerva
{
	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowProperties& properties);
		~WindowsWindow();
		WindowsWindow(const WindowsWindow& other) = delete;
		WindowsWindow& operator=(const WindowsWindow& other) = delete;

		void onUpdate() override;

		unsigned int getWidth() const override;
		unsigned int getHeight() const override;
		void setVSync(bool enabled) override;
		bool isVSync() const override;
		const EventBuffer& getEventBuffer() const override;
		void* getNativeWindow() override { return static_cast<void*>(m_window); }

		bool isKeyPressed(Key key) const override;
		bool isMouseButtonPressed(MouseButton button) const override;
		std::pair<float, float> getMousePosition() const override;
		float getMouseX() const override;
		float getMouseY() const override;
	private:
		GLFWwindow* m_window;

		struct WindowData
		{
			std::string title;
			unsigned int width;
			unsigned int height;
			bool VSync;
			EventBuffer eventBuffer;
		};

		WindowData m_data;
	};

	inline bool WindowsWindow::isKeyPressed(Key key) const
	{
		auto state = glfwGetKey(m_window, static_cast<int>(key));
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	inline bool WindowsWindow::isMouseButtonPressed(MouseButton button) const
	{
		auto state = glfwGetMouseButton(m_window, static_cast<int>(button));
		return state == GLFW_PRESS;
	}

	inline std::pair<float, float> WindowsWindow::getMousePosition() const
	{
		double x, y;
		glfwGetCursorPos(m_window, &x, &y);
		return { static_cast<float>(x), static_cast<float>(y) };
	}

	inline float WindowsWindow::getMouseX() const
	{
		auto [x, y] = getMousePosition();
		return x;
	}

	inline float WindowsWindow::getMouseY() const
	{
		auto [x, y] = getMousePosition();
		return y;
	}

}