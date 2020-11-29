#include "mnpch.h"
#include "OpenGLContext.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>


namespace Minerva
{

	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		: m_windowHandle(windowHandle)
	{
		MN_CORE_ASSERT(windowHandle, "OpenGLContext::OpenGLContext: windowHandle is null.");

		glfwMakeContextCurrent(m_windowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		MN_CORE_ASSERT(status, "WindowsWindow::WindowsWindow: could not initialise glad.");
	}

	void OpenGLContext::swapBuffers()
	{
		glfwSwapBuffers(m_windowHandle);
	}

}