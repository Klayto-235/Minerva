#pragma once

#include "Minerva/Window.h"

#define GLFW_INCLUDE_NONE
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
}