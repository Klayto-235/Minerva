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

		MN_CORE_INFO("OpenGL Info:\n\tVendor: {0}\n\tRenderer: {1}\n\tVersion: {2}",
			glGetString(GL_VENDOR), glGetString(GL_RENDERER), glGetString(GL_VERSION));
		GLCALL(;); // Check for errors.
	}

	void OpenGLContext::swapBuffers()
	{
		glfwSwapBuffers(m_windowHandle);
	}

}