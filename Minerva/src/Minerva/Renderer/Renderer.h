#pragma once

#include "RenderCommand.h"
#include "Shader.h"
#include "Camera.h"

namespace Minerva
{

	class Renderer
	{
	public:
		static void beginScene(const OrthographicCamera& camera);
		static void endScene();

		static void submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray);

		static RenderAPI::API getAPI() { return RenderAPI::getAPI(); }
	private:
		struct SceneData
		{
			glm::mat4 viewProjectionMatrix;
		};

		static SceneData* s_sceneData;
	};

}