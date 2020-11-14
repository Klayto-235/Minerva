#include "mnpch.h"

#include "WindowsWindow.h"


namespace Minerva
{

	WindowsWindow::WindowsWindow(const WindowProperties& properties)
		: m_title(properties.title), m_width(properties.width), m_height(properties.height)
	{
		MN_CORE_INFO("Creating window {0} ({1} x {2}).", m_title, m_width, m_height);

		m_window = glfwCreateWindow(m_width, m_height, m_title.c_str(), nullptr, nullptr);
		MN_CORE_ASSERT(m_window, "Could not create window {0}.", m_title);

		glfwMakeContextCurrent(m_window);
		setVSync(true);
	}

	WindowsWindow::~WindowsWindow()
	{
		MN_CORE_INFO("Destroying window {0}.", m_title);
		glfwDestroyWindow(m_window);
	}

	void WindowsWindow::onUpdate()
	{
		glfwSwapBuffers(m_window);
	}

	unsigned int WindowsWindow::getWidth() const
	{
		return m_width;
	}

	unsigned int WindowsWindow::getHeight() const
	{
		return m_height;
	}

	void WindowsWindow::setVSync(bool enabled)
	{
		if (enabled) glfwSwapInterval(1);
		else glfwSwapInterval(0);

		m_VSync = enabled;
	}

	bool WindowsWindow::isVSync() const
	{
		return m_VSync;
	}

	const EventBuffer& WindowsWindow::getEventBuffer() const
	{
		return m_eventBuffer;
	}

#ifdef MN_PLATFORM_WINDOWS
	inline void Window::init()
	{
		int success = glfwInit();
		MN_CORE_ASSERT(success, "Could not intialise GLFW.");
	}

	inline Window* Window::create(const WindowProperties& properties)
	{
		return new WindowsWindow(properties);
	}

	inline void Window::pollEvents()
	{
		glfwPollEvents();
	}
#endif // MN_PLATFORM_WINDOWS

}