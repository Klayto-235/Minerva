#include "mnpch.h"

#if defined MN_PLATFORM_LINUX

#include "Minerva/core/Window.h"
#include "Minerva/Events/WindowEvent.h"
#include "Minerva/Events/KeyEvent.h"
#include "Minerva/Events/MouseEvent.h"
#include "Platform/OpenGL/OpenGLContext.h"
#include "Minerva/Renderer/RenderAPI.h"

#include <GLFW/glfw3.h>


namespace Minerva
{

	void Window::impl_createWindow()
	{
		MN_PROFILE_FUNCTION();

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#if defined(MN_ENABLE_OPENGL_ERRORS)
		// Yes, the if statement should be here. This code should be RenderAPI agnostic.
		// The rest of the Linux Window code should be rewritten with this in mind.
		if (RenderAPI::getAPI() == RenderAPI::API::OpenGL)
			glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#endif

		GLFWwindow* windowHandle = glfwCreateWindow(m_data.width, m_data.height, m_data.title.c_str(), nullptr, nullptr);
		MN_CORE_ASSERT(windowHandle, "Could not create window \"{0}\".", m_data.title);
		m_nativeWindowHandle = windowHandle;

		m_graphicsContext = createScope<OpenGLContext>(windowHandle);

		glfwSetWindowUserPointer(windowHandle, &m_data);
		setVSync(true);

		glfwSetWindowSizeCallback(windowHandle, [](GLFWwindow* windowHandle, int width, int height)
		{
			WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(windowHandle));
			data.width = width;
			data.height = height;
			data.eventBuffer.add<WindowResizeEvent>(width, height);
		});

		glfwSetWindowCloseCallback(windowHandle, [](GLFWwindow* windowHandle)
		{
			WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(windowHandle));
			data.eventBuffer.add<WindowCloseEvent>(nullptr);
		});

		glfwSetWindowFocusCallback(windowHandle, [](GLFWwindow* windowHandle, int focused)
		{
			WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(windowHandle));
			if (focused == GLFW_FALSE)
				data.inputState.onLoseFocus();
		});

		glfwSetKeyCallback(windowHandle, [](GLFWwindow* windowHandle, int key, int scancode, int action, int mods)
		{
			WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(windowHandle));
			switch (action)
			{
			case GLFW_PRESS:
				data.eventBuffer.add<KeyPressEvent>(static_cast<Key>(key), false);
				data.inputState.setKey(static_cast<Key>(key), true);
				break;

			case GLFW_RELEASE:
				data.eventBuffer.add<KeyReleaseEvent>(static_cast<Key>(key));
				data.inputState.setKey(static_cast<Key>(key), false);
				break;

			case GLFW_REPEAT:
				data.eventBuffer.add<KeyPressEvent>(static_cast<Key>(key), true);
				break;
			}
		});

		glfwSetCharCallback(windowHandle, [](GLFWwindow* windowHandle, unsigned int character)
		{
			WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(windowHandle));
			data.eventBuffer.add<TextCharEvent>(static_cast<int>(character));
		});

		glfwSetMouseButtonCallback(windowHandle, [](GLFWwindow* windowHandle, int button, int action, int mods)
		{
			WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(windowHandle));
			switch (action)
			{
			case GLFW_PRESS:
				data.eventBuffer.add<MouseButtonPressEvent>(static_cast<MouseButton>(button));
				data.inputState.setMouseButton(static_cast<MouseButton>(button), true);
				break;

			case GLFW_RELEASE:
				data.eventBuffer.add<MouseButtonReleaseEvent>(static_cast<MouseButton>(button));
				data.inputState.setMouseButton(static_cast<MouseButton>(button), false);
				break;
			}
		});

		glfwSetScrollCallback(windowHandle, [](GLFWwindow* windowHandle, double xOffset, double yOffset)
		{
			WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(windowHandle));
			data.eventBuffer.add<MouseScrollEvent>((float)xOffset, (float)yOffset);
		});

		glfwSetCursorPosCallback(windowHandle, [](GLFWwindow* windowHandle, double x, double y)
		{
			WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(windowHandle));
			data.eventBuffer.add<MouseMoveEvent>((float)x, (float)y);
			data.inputState.setMousePosition((float)x, (float)y);
		});
	}

	void Window::impl_destroyWindow()
	{
		MN_PROFILE_FUNCTION();

		glfwDestroyWindow(static_cast<GLFWwindow*>(m_nativeWindowHandle));
	}

	void Window::setVSync(bool enable)
	{
		MN_PROFILE_FUNCTION();

		// Retain current context because this is exposed in public Window API.
		GLFWwindow* currentContext = glfwGetCurrentContext();
		glfwMakeContextCurrent(static_cast<GLFWwindow*>(m_nativeWindowHandle));
		if (enable) glfwSwapInterval(1);
		else glfwSwapInterval(0);
		m_VSync = enable;
		glfwMakeContextCurrent(currentContext);
	}

	void Window::init()
	{
		MN_PROFILE_FUNCTION();

		glfwSetErrorCallback([](int error, const char* description)
		{
			MN_CORE_ERROR("GLFW Error ({0}): {1}.", error, description);
		});
		int success = glfwInit();
		MN_CORE_ASSERT(success, "Could not intialise GLFW.");
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

}

#endif // MN_PLATFORM_LINUX