#include "mnpch.h"
#include "Renderer.h"

#include "Platform/OpenGL/OpenGLShader.h" // TEMPORARY

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

	void Renderer::submit(const std::shared_ptr<Shader>& shader,
		const std::shared_ptr<VertexArray>& vertexArray, const glm::mat4& transform)
	{
		shader->bind();
		std::dynamic_pointer_cast<OpenGLShader>(shader)->
			uploadUniformMat4("u_VP", s_sceneData->viewProjectionMatrix);
		std::dynamic_pointer_cast<OpenGLShader>(shader)->
			uploadUniformMat4("u_M", transform);

		vertexArray->bind();
		RenderCommand::drawIndexed(vertexArray);
	}

}