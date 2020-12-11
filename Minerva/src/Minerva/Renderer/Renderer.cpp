#include "mnpch.h"
#include "Renderer.h"

#include "Platform/OpenGL/OpenGLShader.h" // TEMPORARY

namespace Minerva
{
	Renderer::SceneData* Renderer::s_sceneData = new Renderer::SceneData;

	void Renderer::init()
	{
		RenderCommand::init();
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
		std::dynamic_pointer_cast<OpenGLShader>(shader)->
			uploadUniformMat4("u_VP", s_sceneData->viewProjectionMatrix);
		std::dynamic_pointer_cast<OpenGLShader>(shader)->
			uploadUniformMat4("u_M", transform);

		vertexArray->bind();
		RenderCommand::drawIndexed(vertexArray);
	}

}