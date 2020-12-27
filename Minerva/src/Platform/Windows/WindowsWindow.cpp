#include "mnpch.h"
#include "Platform/Windows/WindowsWindow.h"
#include "Minerva/Events/WindowEvent.h"
#include "Minerva/Events/KeyEvent.h"
#include "Minerva/Events/MouseEvent.h"
#include "Platform/OpenGL/OpenGLContext.h"
#include "Minerva/Renderer/RenderAPI.h"


namespace Minerva
{

#pragma region /////////////// WindowsWindowInputState ///////////////

	bool WindowsWindowInputState::isKeyPressed(Key key) const
	{
		auto state = glfwGetKey(m_window.m_windowHandle, static_cast<int>(key));
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool WindowsWindowInputState::isMouseButtonPressed(MouseButton button) const
	{
		auto state = glfwGetMouseButton(m_window.m_windowHandle, static_cast<int>(button));
		return state == GLFW_PRESS;
	}

	std::pair<float, float> WindowsWindowInputState::getMousePosition() const
	{
		double x, y;
		glfwGetCursorPos(m_window.m_windowHandle, &x, &y);
		return { static_cast<float>(x), static_cast<float>(y) };
	}

	float WindowsWindowInputState::getMouseX() const
	{
		auto [x, y] = getMousePosition();
		return x;
	}

	float WindowsWindowInputState::getMouseY() const
	{
		auto [x, y] = getMousePosition();
		return y;
	}

#pragma endregion

#pragma region /////////////// WindowsWindow ///////////////

	WindowsWindow::WindowsWindow(const WindowProperties& properties)
		: Window(properties), m_inputState(*this)
	{
		MN_PROFILE_FUNCTION();

		MN_CORE_INFO("Creating window \"{0}\" ({1} x {2}).", m_data.title, m_data.width, m_data.height);

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#if defined(MN_ENABLE_OPENGL_ERRORS)
		if (RenderAPI::getAPI() == RenderAPI::API::OpenGL)
			glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#endif

		m_windowHandle = glfwCreateWindow(m_data.width, m_data.height, m_data.title.c_str(), nullptr, nullptr);
		MN_CORE_ASSERT(m_windowHandle, "WindowsWindow::WindowsWindow: Could not create window \"{0}\".", m_data.title);

		m_context = createScope<OpenGLContext>(m_windowHandle);

		glfwSetWindowUserPointer(m_windowHandle, &m_data);
		setVSync(true);

		glfwSetWindowSizeCallback(m_windowHandle, [](GLFWwindow* windowHandle, int width, int height)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(windowHandle);
			data.width = width;
			data.height = height;
			data.eventBuffer.add<WindowResizeEvent>(width, height);
		});

		glfwSetWindowCloseCallback(m_windowHandle, [](GLFWwindow* windowHandle)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(windowHandle);
			data.eventBuffer.add<WindowCloseEvent>(nullptr);
		});

		glfwSetKeyCallback(m_windowHandle, [](GLFWwindow* windowHandle, int key, int scancode, int action, int mods)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(windowHandle);
			switch (action)
			{
			case GLFW_PRESS:
				data.eventBuffer.add<KeyPressEvent>(static_cast<Key>(key), false);
				break;

			case GLFW_RELEASE:
				data.eventBuffer.add<KeyReleaseEvent>(static_cast<Key>(key));
				break;

			case GLFW_REPEAT:
				data.eventBuffer.add<KeyPressEvent>(static_cast<Key>(key), true);
				break;
			}
		});

		glfwSetCharCallback(m_windowHandle, [](GLFWwindow* windowHandle, unsigned int character)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(windowHandle);
			data.eventBuffer.add<TextCharEvent>(static_cast<int>(character));
		});

		glfwSetMouseButtonCallback(m_windowHandle, [](GLFWwindow* windowHandle, int button, int action, int mods)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(windowHandle);
			switch (action)
			{
			case GLFW_PRESS:
				data.eventBuffer.add<MouseButtonPressEvent>(static_cast<MouseButton>(button));
				break;

			case GLFW_RELEASE:
				data.eventBuffer.add<MouseButtonReleaseEvent>(static_cast<MouseButton>(button));
				break;
			}
		});

		glfwSetScrollCallback(m_windowHandle, [](GLFWwindow* windowHandle, double xOffset, double yOffset)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(windowHandle);
			data.eventBuffer.add<MouseScrollEvent>((float)xOffset, (float)yOffset);
		});

		glfwSetCursorPosCallback(m_windowHandle, [](GLFWwindow* windowHandle, double x, double y)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(windowHandle);
			data.eventBuffer.add<MouseMoveEvent>((float)x, (float)y);
		});

		initResources();
	}

	WindowsWindow::~WindowsWindow()
	{
		MN_PROFILE_FUNCTION();

		MN_CORE_INFO("Destroying window \"{0}\".", m_data.title);
		glfwMakeContextCurrent(m_windowHandle);
		freeResources(); // Free OpenGL resources before context destruction.
		glfwDestroyWindow(m_windowHandle);
	}

	void WindowsWindow::setVSync(bool enabled)
	{
		MN_PROFILE_FUNCTION();

		// Retain context because this is exposed in public Window API.
		GLFWwindow* currentContext = glfwGetCurrentContext();
		glfwMakeContextCurrent(m_windowHandle);
		if (enabled) glfwSwapInterval(1);
		else glfwSwapInterval(0);
		m_data.VSync = enabled;
		glfwMakeContextCurrent(currentContext);
	}

#pragma endregion

#pragma region /////////////// Window ///////////////

#ifdef MN_PLATFORM_WINDOWS
	void Window::init()
	{
		MN_PROFILE_FUNCTION();

		glfwSetErrorCallback([](int error, const char* description)
		{
			MN_CORE_ERROR("GLFW Error ({0}): {1}.", error, description);
		});
		int success = glfwInit();
		MN_CORE_ASSERT(success, "Window::init: Could not intialise GLFW.");
	}

	Scope<Window> Window::create(const WindowProperties& properties)
	{
		return createScope<WindowsWindow>(properties);
	}

	void Window::pollEvents()
	{
		MN_PROFILE_FUNCTION();

		glfwPollEvents();
	}

	void Window::terminate()
	{
		MN_PROFILE_FUNCTION();

		glfwTerminate();
	}
#endif // MN_PLATFORM_WINDOWS

#pragma endregion

}