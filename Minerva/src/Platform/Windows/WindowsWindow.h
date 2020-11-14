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

		virtual void onUpdate() override;

		virtual unsigned int getWidth() const override;
		virtual unsigned int getHeight() const override;
		virtual void setVSync(bool enabled) override;
		virtual bool isVSync() const override;
		virtual const EventBuffer& getEventBuffer() const override;
	private:
		std::string m_title;
		unsigned int m_width;
		unsigned int m_height;
		bool m_VSync;
		EventBuffer m_eventBuffer;
		GLFWwindow* m_window;
	};
}