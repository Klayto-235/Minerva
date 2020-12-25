#include "mnpch.h"
#include "Platform/OpenGL/OpenGLRenderAPI.h"
#include "Platform/OpenGL/OpenGL_core.h"


namespace Minerva
{
	void OpenGLRenderAPI::init()
	{
		GLCALL(glEnable(GL_BLEND));
		GLCALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		GLCALL(glEnable(GL_DEPTH_TEST));
	}

	void OpenGLRenderAPI::setViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		glViewport(x, y, width, height);
	}

	void OpenGLRenderAPI::setClearColor(const glm::vec4& color)
	{
		GLCALL(glClearColor(color.r, color.g, color.b, color.a));
	}

	void OpenGLRenderAPI::clear()
	{
		GLCALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	}

	void OpenGLRenderAPI::drawIndexed(const Ref<VertexArray>& vertexArray)
	{
		GLCALL(glDrawElements(GL_TRIANGLES, vertexArray->getIndexBuffer()->getCount(), GL_UNSIGNED_INT, nullptr));
	}

}