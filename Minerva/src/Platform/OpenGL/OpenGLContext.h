#pragma once

#include "Minerva/Renderer/GraphicsContext.h"


struct GLFWwindow;


namespace Minerva
{

	class OpenGLContext : public GraphicsContext
	{
	public:
		explicit OpenGLContext(GLFWwindow* windowHandle);

		virtual void swapBuffers() override;
		virtual void makeCurrent() const override;
	private:
		GLFWwindow* m_windowHandle;
	};

}