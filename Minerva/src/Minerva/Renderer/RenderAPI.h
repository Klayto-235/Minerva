#pragma once

#include "Minerva/core/core.h"
#include "Minerva/Renderer/Buffer.h"

#include <glm/glm.hpp>


namespace Minerva
{

	class RenderAPI
	{
	public:
		enum class API
		{
			None = 0, OpenGL
		};

		RenderAPI() = default;
		RenderAPI(const RenderAPI&) = delete;
		RenderAPI& operator=(const RenderAPI&) = delete;
		virtual ~RenderAPI() = default;

		virtual void init() = 0;
		virtual void setViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;

		virtual void setClearColor(const glm::vec4& color) = 0;
		virtual void clear() = 0;

		virtual void drawIndexed(const Ref<VertexArray>& vertexArray) = 0;

		static Scope<RenderAPI> create();
		static API getAPI() { return s_API; }
	private:
		static API s_API;
	};

}