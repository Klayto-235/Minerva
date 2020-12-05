#pragma once

#include "RenderCommand.h"


namespace Minerva
{

	class Renderer
	{
	public:
		static void beginScene();
		static void endScene();

		static void submit(const std::shared_ptr<VertexArray>& vertexArray);

		static RenderAPI::API getAPI() { return RenderAPI::getAPI(); }
	};

}