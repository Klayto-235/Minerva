#include "mnpch.h"
#include "Platform/OpenGL/OpenGLContext.h"
#include "Platform/OpenGL/OpenGL_core.h"

#include <GLFW/glfw3.h>


namespace Minerva
{

	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		: m_windowHandle(windowHandle)
	{
		MN_CORE_ASSERT(windowHandle, "OpenGLContext::OpenGLContext: WindowHandle is null.");

#if defined MN_ENABLE_ASSERTS
		auto* glViewport_old = glViewport;
#endif
		glfwMakeContextCurrent(m_windowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		MN_CORE_ASSERT(status, "OpenGLContext::OpenGLContext: Could not initialise glad.");
		// Attempt to detect a change in function pointers for multi-context support.
		MN_CORE_ASSERT(glViewport_old == nullptr || glViewport_old == glViewport,
			"OpenGLContext::OpenGLContext: OpenGL function pointers have changed.");

		MN_CORE_INFO("OpenGL Info:\n\tVendor: {0}\n\tRenderer: {1}\n\tVersion: {2}",
			glGetString(GL_VENDOR), glGetString(GL_RENDERER), glGetString(GL_VERSION));
		GLCALL(;); // Check for errors.

#if defined MN_ENABLE_ASSERTS
		int versionMajor;
		int versionMinor;
		glGetIntegerv(GL_MAJOR_VERSION, &versionMajor);
		glGetIntegerv(GL_MINOR_VERSION, &versionMinor);
		MN_CORE_ASSERT(versionMajor > 4 || (versionMajor == 4 && versionMinor >= 5),
			"OpenGLContext::OpenGLContext: Minerva requires at least OpenGL version 4.5.");
#endif
	}

	void OpenGLContext::swapBuffers()
	{
		glfwSwapBuffers(m_windowHandle);
	}

}