#pragma once

#include "Minerva/Renderer/RenderAPI.h"


namespace Minerva
{

	class OpenGLRenderAPI : public RenderAPI
	{
	public:
		void init() override;
		void setViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;

		void setClearColor(const glm::vec4& color) override;
		void clear() override;

		void drawIndexed(const Ref<VertexArray>& vertexArray) override;
	};

}