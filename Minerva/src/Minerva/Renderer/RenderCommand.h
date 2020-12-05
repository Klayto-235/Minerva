#pragma once

#include "RenderAPI.h"


namespace Minerva
{

	class RenderCommand
	{
	public:
		static void setClearColor(const glm::vec4& color)
		{
			s_renderAPI->setClearColor(color);
		}
		static void clear()
		{
			s_renderAPI->clear();
		}

		static void drawIndexed(const std::shared_ptr<VertexArray>& vertexArray)
		{
			s_renderAPI->drawIndexed(vertexArray);
		}
	private:
		static RenderAPI* s_renderAPI;
	};

}