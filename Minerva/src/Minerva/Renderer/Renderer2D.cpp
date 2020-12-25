#include "mnpch.h"
#include "Minerva/Renderer/Renderer2D.h"
#include "Minerva/Renderer/RenderCommand.h"

#include "Platform/OpenGL/OpenGLShader.h" // TEMPORARY


namespace Minerva
{

	Renderer2D::Renderer2D()
	{
		m_squareVertexArray = VertexArray::create();

		float squareVertices[3 * 4] = {
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.5f,  0.5f, 0.0f,
			-0.5f,  0.5f, 0.0f
		};

		Ref<VertexBuffer> squareVertexBuffer(VertexBuffer::create(squareVertices, sizeof(squareVertices)));
		squareVertexBuffer->setLayout({
			{ ShaderDataType::Float3, "a_position" }
		});
		m_squareVertexArray->addVertexBuffer(squareVertexBuffer);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		Ref<IndexBuffer> squareIndexBuffer(IndexBuffer::create(squareIndices,
			sizeof(squareIndices) / sizeof(squareIndices[0])));
		m_squareVertexArray->setIndexBuffer(squareIndexBuffer);

		m_flatColorShader = Minerva::Shader::create("assets/shaders/flat_color.glsl");
	}

	void Renderer2D::beginScene(const OrthographicCamera& camera)
	{
		std::dynamic_pointer_cast<OpenGLShader>(m_flatColorShader)->bind();
		std::dynamic_pointer_cast<OpenGLShader>(m_flatColorShader)->
			uploadUniformMat4("u_VP", camera.getViewProjectionMatrix());
		std::dynamic_pointer_cast<OpenGLShader>(m_flatColorShader)->
			uploadUniformMat4("u_M", glm::mat4(1.0f));
	}

	void Renderer2D::endScene()
	{
	}

	void Renderer2D::drawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		drawQuad({ position.x, position.y, 0.0f }, size, color);
	}

	void Renderer2D::drawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		std::dynamic_pointer_cast<OpenGLShader>(m_flatColorShader)->bind();
		std::dynamic_pointer_cast<OpenGLShader>(m_flatColorShader)->uploadUniformFloat4("u_color", color);

		m_squareVertexArray->bind();
		RenderCommand::drawIndexed(m_squareVertexArray);
	}

}