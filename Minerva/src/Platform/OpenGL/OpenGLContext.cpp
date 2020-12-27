#include "mnpch.h"
#include "Platform/OpenGL/OpenGLContext.h"

#include <glad/glad.h>

#include <GLFW/glfw3.h>


namespace Minerva
{

#ifdef MN_ENABLE_OPENGL_ERRORS
	void OpenGLDebugMessageCallback(
		unsigned source,
		unsigned type,
		unsigned id,
		unsigned severity,
		int length,
		const char* message,
		const void* userParam)
	{
		switch (severity)
		{
		case GL_DEBUG_SEVERITY_HIGH:			MN_CORE_CRITICAL(message); break;
		case GL_DEBUG_SEVERITY_MEDIUM:			MN_CORE_ERROR(message); break;
		case GL_DEBUG_SEVERITY_LOW:				MN_CORE_WARN(message); break;
		case GL_DEBUG_SEVERITY_NOTIFICATION:	MN_CORE_TRACE(message); break;
		default: MN_CORE_ASSERT(false, "OpenGLDebugMessageCallback: Unknown severity level.");
		}

#ifdef MN_ENABLE_OPENGL_ASSERTS
		MN_CORE_ASSERT(false, "OpenGL error (context {0}).", userParam);
#endif
	}
#endif

	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		: m_windowHandle(windowHandle)
	{
		MN_PROFILE_FUNCTION();

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

#if defined MN_ENABLE_ASSERTS
		int versionMajor;
		int versionMinor;
		glGetIntegerv(GL_MAJOR_VERSION, &versionMajor);
		glGetIntegerv(GL_MINOR_VERSION, &versionMinor);
		MN_CORE_ASSERT(versionMajor > 4 || (versionMajor == 4 && versionMinor >= 5),
			"OpenGLContext::OpenGLContext: Minerva requires at least OpenGL version 4.5.");
#endif

#ifdef MN_ENABLE_OPENGL_ERRORS
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(OpenGLDebugMessageCallback, static_cast<void*>(this));

		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);
#endif
	}

	void OpenGLContext::swapBuffers()
	{
		MN_PROFILE_FUNCTION();

		glfwSwapBuffers(m_windowHandle);
	}

	void OpenGLContext::makeCurrent() const
	{
		MN_PROFILE_FUNCTION();

		glfwMakeContextCurrent(m_windowHandle);
	}

}