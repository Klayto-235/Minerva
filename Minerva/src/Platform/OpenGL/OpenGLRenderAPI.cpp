#include "mnpch.h"
#include "Platform/OpenGL/OpenGLRenderAPI.h"

#include <glad/glad.h>


namespace Minerva
{

	void OpenGLRenderAPI::init()
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_DEPTH_TEST);
	}

	void OpenGLRenderAPI::setViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		glViewport(x, y, width, height);
	}

	void OpenGLRenderAPI::setClearColor(const glm::vec4& color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void OpenGLRenderAPI::clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLRenderAPI::drawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexOffset, uint32_t indexCount)
	{
		MN_CORE_ASSERT(indexCount + indexOffset <= vertexArray->getIndexBuffer()->getCount(),
			"index interval exceeds bounds of index buffer.");

		glDrawElements(GL_TRIANGLES, (indexCount > 0) ? indexCount :
			(vertexArray->getIndexBuffer()->getCount() - indexOffset),
			GL_UNSIGNED_INT, reinterpret_cast<const void*>((uint64_t)indexOffset));
	}

}