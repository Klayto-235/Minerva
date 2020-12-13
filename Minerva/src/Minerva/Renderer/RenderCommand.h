#pragma once

#include "RenderAPI.h"


namespace Minerva
{

	class RenderCommand
	{
	public:
		static void init()
		{
			s_renderAPI->init();
		}

		static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
		{
			s_renderAPI->setViewport(x, y, width, height);
		}

		static void setClearColor(const glm::vec4& color)
		{
			s_renderAPI->setClearColor(color);
		}
		static void clear()
		{
			s_renderAPI->clear();
		}

		static void drawIndexed(const Ref<VertexArray>& vertexArray)
		{
			s_renderAPI->drawIndexed(vertexArray);
		}
	private:
		static RenderAPI* s_renderAPI;
	};

}