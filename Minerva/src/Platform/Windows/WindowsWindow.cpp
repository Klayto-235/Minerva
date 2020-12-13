#include "mnpch.h"
#include "WindowsWindow.h"
#include "Minerva/Events/WindowEvent.h"
#include "Minerva/Events/KeyEvent.h"
#include "Minerva/Events/MouseEvent.h"
#include "Platform/OpenGL/OpenGLContext.h"


namespace Minerva
{

	bool WindowsWindow::WindowsInputState::isKeyPressed(Key key) const
	{
		auto state = glfwGetKey(m_window.m_window, static_cast<int>(key));
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool WindowsWindow::WindowsInputState::isMouseButtonPressed(MouseButton button) const
	{
		auto state = glfwGetMouseButton(m_window.m_window, static_cast<int>(button));
		return state == GLFW_PRESS;
	}

	std::pair<float, float> WindowsWindow::WindowsInputState::getMousePosition() const
	{
		double x, y;
		glfwGetCursorPos(m_window.m_window, &x, &y);
		return { static_cast<float>(x), static_cast<float>(y) };
	}

	float WindowsWindow::WindowsInputState::getMouseX() const
	{
		auto [x, y] = getMousePosition();
		return x;
	}

	float WindowsWindow::WindowsInputState::getMouseY() const
	{
		auto [x, y] = getMousePosition();
		return y;
	}

	WindowsWindow::WindowsWindow(const WindowProperties& properties)
		: m_data{ properties.title, properties.width, properties.height }, m_inputState(*this)
	{
		MN_CORE_INFO("Creating window \"{0}\" ({1} x {2}).", m_data.title, m_data.width, m_data.height);

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		m_window = glfwCreateWindow(m_data.width, m_data.height, m_data.title.c_str(), nullptr, nullptr);
		MN_CORE_ASSERT(m_window, "WindowsWindow::WindowsWindow: Could not create window \"{0}\".", m_data.title);

		m_context = std::make_unique<OpenGLContext>(m_window);

		glfwSetWindowUserPointer(m_window, &m_data);
		setVSync(true);

		glfwSetWindowSizeCallback(m_window, [](GLFWwindow* window, int width, int height)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			data.width = width;
			data.height = height;
			data.eventBuffer.post<WindowResizeEvent>(width, height);
		});

		glfwSetWindowCloseCallback(m_window, [](GLFWwindow* window)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			data.eventBuffer.post<WindowCloseEvent>();
		});

		glfwSetKeyCallback(m_window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			switch (action)
			{
			case GLFW_PRESS:
				data.eventBuffer.post<KeyPressEvent>(static_cast<Key>(key), false);
				break;

			case GLFW_RELEASE:
				data.eventBuffer.post<KeyReleaseEvent>(static_cast<Key>(key));
				break;

			case GLFW_REPEAT:
				data.eventBuffer.post<KeyPressEvent>(static_cast<Key>(key), true);
				break;
			}
		});

		glfwSetCharCallback(m_window, [](GLFWwindow* window, unsigned int character)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			data.eventBuffer.post<TextCharEvent>(static_cast<int>(character));
		});

		glfwSetMouseButtonCallback(m_window, [](GLFWwindow* window, int button, int action, int mods)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			switch (action)
			{
			case GLFW_PRESS:
				data.eventBuffer.post<MouseButtonPressEvent>(static_cast<MouseButton>(button));
				break;

			case GLFW_RELEASE:
				data.eventBuffer.post<MouseButtonReleaseEvent>(static_cast<MouseButton>(button));
				break;
			}
		});

		glfwSetScrollCallback(m_window, [](GLFWwindow* window, double xOffset, double yOffset)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			data.eventBuffer.post<MouseScrollEvent>((float)xOffset, (float)yOffset);
		});

		glfwSetCursorPosCallback(m_window, [](GLFWwindow* window, double x, double y)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			data.eventBuffer.post<MouseMoveEvent>((float)x, (float)y);
		});
	}

	WindowsWindow::~WindowsWindow()
	{
		MN_CORE_INFO("Destroying window \"{0}\".", m_data.title);
		glfwDestroyWindow(m_window);
	}

	void WindowsWindow::onUpdate()
	{
		m_context->swapBuffers();
		m_data.eventBuffer.clear();
	}

	void WindowsWindow::setVSync(bool enabled)
	{
		if (enabled) glfwSwapInterval(1);
		else glfwSwapInterval(0);

		m_data.VSync = enabled;
	}

#ifdef MN_PLATFORM_WINDOWS
	void Window::init()
	{
		glfwSetErrorCallback([](int error, const char* description)
		{
			MN_CORE_ERROR("GLFW Error ({0}): {1}.", error, description);
		});
		int success = glfwInit();
		MN_CORE_ASSERT(success, "Window::init: Could not intialise GLFW.");
	}

	Window* Window::create(const WindowProperties& properties)
	{
		return new WindowsWindow(properties);
	}

	void Window::pollEvents()
	{
		glfwPollEvents();
	}

	void Window::terminate()
	{
		glfwTerminate();
	}
#endif // MN_PLATFORM_WINDOWS

}