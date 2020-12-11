#pragma once

#include "RenderCommand.h"
#include "Shader.h"
#include "Camera.h"

namespace Minerva
{

	class Renderer
	{
	public:
		static void init();

		static void beginScene(const OrthographicCamera& camera);
		static void endScene();

		static void submit(const Ref<Shader>& shader,
			const Ref<VertexArray>& vertexArray, const glm::mat4& transform);

		static RenderAPI::API getAPI() { return RenderAPI::getAPI(); }
	private:
		struct SceneData
		{
			glm::mat4 viewProjectionMatrix;
		};

		static SceneData* s_sceneData;
	};

}