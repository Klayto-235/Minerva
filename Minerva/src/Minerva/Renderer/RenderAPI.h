#pragma once

#include "Buffer.h"

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

		virtual void setClearColor(const glm::vec4& color) = 0;
		virtual void clear() = 0;

		virtual void drawIndexed(const Ref<VertexArray>& vertexArray) = 0;

		static API getAPI() { return s_API; }
	private:
		static API s_API;
	};

}