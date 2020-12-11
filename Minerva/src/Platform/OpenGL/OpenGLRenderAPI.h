#pragma once

#include "Minerva/Renderer/RenderAPI.h"


namespace Minerva
{

	class OpenGLRenderAPI : public RenderAPI
	{
	public:
		void init() override;

		void setClearColor(const glm::vec4& color) override;
		void clear() override;

		void drawIndexed(const Ref<VertexArray>& vertexArray) override;
	};

}