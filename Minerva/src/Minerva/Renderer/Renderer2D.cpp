#include "mnpch.h"
#include "Minerva/Renderer/Renderer2D.h"
#include "Minerva/Renderer/RenderCommand.h"

#include <glm/gtc/matrix_transform.hpp>


namespace Minerva
{

	Renderer2D::Renderer2D()
	{
		MN_PROFILE_FUNCTION();

		m_squareVertexArray = VertexArray::create();

		float squareVertices[5 * 4] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
		};

		Ref<VertexBuffer> squareVertexBuffer(VertexBuffer::create(squareVertices, sizeof(squareVertices)));
		squareVertexBuffer->setLayout({
			{ ShaderDataType::Float3, "a_position" },
			{ ShaderDataType::Float2, "a_texCoord" }
		});
		m_squareVertexArray->addVertexBuffer(squareVertexBuffer);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		Ref<IndexBuffer> squareIndexBuffer(IndexBuffer::create(squareIndices,
			sizeof(squareIndices) / sizeof(squareIndices[0])));
		m_squareVertexArray->setIndexBuffer(squareIndexBuffer);

		uint32_t data = 0xffffffff;
		m_whiteTexture = Texture2D::create(1, 1);
		m_whiteTexture->setData(&data, sizeof(uint32_t));

		m_textureShader = Minerva::Shader::create("assets/shaders/texture.glsl");
		m_textureShader->bind();
		m_textureShader->setInt("u_texture", 0);
	}

	Renderer2D::~Renderer2D()
	{
		MN_PROFILE_FUNCTION();
	}

	void Renderer2D::beginScene(const OrthographicCamera& camera)
	{
		MN_PROFILE_FUNCTION();

		m_textureShader->bind();
		m_textureShader->setMat4("u_VP", camera.getViewProjectionMatrix());
	}

	void Renderer2D::endScene()
	{
		MN_PROFILE_FUNCTION();
	}

	void Renderer2D::drawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		MN_PROFILE_FUNCTION();

		m_textureShader->bind();
		m_textureShader->setFloat4("u_color", color);

		const glm::mat4 transform = glm::scale(
			glm::translate(glm::mat4(1.0f), position), { size.x, size.y, 1.0f });
		m_textureShader->setMat4("u_M", transform);

		m_whiteTexture->bind();

		m_squareVertexArray->bind();
		RenderCommand::drawIndexed(m_squareVertexArray);
	}

	void Renderer2D::drawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		drawQuad({ position.x, position.y, 0.0f }, size, color);
	}

	void Renderer2D::drawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture,
		float tilingFactor, const glm::vec4& tintColor)
	{
		MN_PROFILE_FUNCTION();

		m_textureShader->bind();
		m_textureShader->setFloat4("u_color", tintColor);
		m_textureShader->setFloat("u_tilingFactor", tilingFactor);

		const glm::mat4 transform = glm::scale(
			glm::translate(glm::mat4(1.0f), position), { size.x, size.y, 1.0f });
		m_textureShader->setMat4("u_M", transform);

		texture->bind();

		m_squareVertexArray->bind();
		RenderCommand::drawIndexed(m_squareVertexArray);
	}

	void Renderer2D::drawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture,
		float tilingFactor, const glm::vec4& tintColor)
	{
		drawQuad({ position.x, position.y, 0.0f }, size, texture, tilingFactor, tintColor);
	}

	void Renderer2D::drawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float angle, const glm::vec4& color)
	{
		MN_PROFILE_FUNCTION();

		m_textureShader->bind();
		m_textureShader->setFloat4("u_color", color);

		const glm::mat4 transform = glm::scale(glm::rotate(glm::translate(glm::mat4(1.0f), position),
			angle, { 0.0f, 0.0f, 1.0f }), { size.x, size.y, 1.0f });
		m_textureShader->setMat4("u_M", transform);

		m_whiteTexture->bind();

		m_squareVertexArray->bind();
		RenderCommand::drawIndexed(m_squareVertexArray);
	}

	void Renderer2D::drawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float angle, const glm::vec4& color)
	{
		drawRotatedQuad({ position.x, position.y, 0.0f }, size, angle, color);
	}

	void Renderer2D::drawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float angle, const Ref<Texture2D>& texture,
		float tilingFactor, const glm::vec4& tintColor)
	{
		MN_PROFILE_FUNCTION();

		m_textureShader->bind();
		m_textureShader->setFloat4("u_color", tintColor);
		m_textureShader->setFloat("u_tilingFactor", tilingFactor);

		const glm::mat4 transform = glm::scale(glm::rotate(glm::translate(glm::mat4(1.0f), position),
			angle, { 0.0f, 0.0f, 1.0f }), { size.x, size.y, 1.0f });
		m_textureShader->setMat4("u_M", transform);

		texture->bind();

		m_squareVertexArray->bind();
		RenderCommand::drawIndexed(m_squareVertexArray);
	}

	void Renderer2D::drawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float angle, const Ref<Texture2D>& texture,
		float tilingFactor, const glm::vec4& tintColor)
	{
		drawRotatedQuad({ position.x, position.y, 0.0f }, size, angle, texture, tilingFactor, tintColor);
	}

}