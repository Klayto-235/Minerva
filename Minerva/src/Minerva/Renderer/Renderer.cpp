#include "mnpch.h"
#include "Renderer.h"

namespace Minerva
{
	Renderer::SceneData* Renderer::s_sceneData = new Renderer::SceneData;

	void Renderer::beginScene(const OrthographicCamera& camera)
	{
		s_sceneData->viewProjectionMatrix = camera.getViewProjectionMatrix();
	}

	void Renderer::endScene()
	{
	}

	void Renderer::submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray)
	{
		shader->bind();
		shader->uploadUniformMat4("u_VP", s_sceneData->viewProjectionMatrix);

		vertexArray->bind();
		RenderCommand::drawIndexed(vertexArray);
	}

}