#include "mnpch.h"
#include "OpenGLRenderAPI.h"
#include "OpenGL_core.h"


namespace Minerva
{

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