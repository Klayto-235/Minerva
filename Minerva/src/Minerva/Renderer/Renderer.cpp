#include "mnpch.h"
#include "Minerva/Renderer/Renderer.h"

namespace Minerva
{
	Scope<Renderer::SceneData> Renderer::s_sceneData = createScope<Renderer::SceneData>();

	void Renderer::init()
	{
		
	}

	void Renderer::onWindowResize(uint32_t width, uint32_t height)
	{
		RenderCommand::SetViewport(0, 0, width, height);
	}

	void Renderer::beginScene(const OrthographicCamera& camera)
	{
		s_sceneData->viewProjectionMatrix = camera.getViewProjectionMatrix();
	}

	void Renderer::endScene()
	{
	}

	void Renderer::submit(const Ref<Shader>& shader,
		const Ref<VertexArray>& vertexArray, const glm::mat4& transform)
	{
		shader->bind();
		shader->setMat4("u_VP", s_sceneData->viewProjectionMatrix);
		shader->setMat4("u_M", transform);

		vertexArray->bind();
		RenderCommand::drawIndexed(vertexArray);
	}

}