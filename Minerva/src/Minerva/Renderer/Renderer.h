#pragma once

#include "Minerva/core/core.h"
#include "Minerva/Renderer/RenderCommand.h"
#include "Minerva/Renderer/Shader.h"
#include "Minerva/Renderer/Camera.h"

namespace Minerva
{

	class Renderer
	{
	public:
		static void init();
		static void onWindowResize(uint32_t width, uint32_t height);

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

		static Scope<SceneData> s_sceneData;
	};

}