#include "mnpch.h"
#include "OpenGLContext.h"
#include "OpenGL_core.h"

#include <GLFW/glfw3.h>


namespace Minerva
{

	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		: m_windowHandle(windowHandle)
	{
		MN_CORE_ASSERT(windowHandle, "OpenGLContext::OpenGLContext: WindowHandle is null.");

		glfwMakeContextCurrent(m_windowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		MN_CORE_ASSERT(status, "WindowsWindow::WindowsWindow: Could not initialise glad.");

		MN_CORE_INFO("OpenGL Info:");
		MN_CORE_INFO("\tVendor: {0}", glGetString(GL_VENDOR));
		MN_CORE_INFO("\tRenderer: {0}", glGetString(GL_RENDERER));
		MN_CORE_INFO("\tVersion: {0}", glGetString(GL_VERSION));
		GLCALL(;); // Check for errors.
	}

	void OpenGLContext::swapBuffers()
	{
		glfwSwapBuffers(m_windowHandle);
	}

}