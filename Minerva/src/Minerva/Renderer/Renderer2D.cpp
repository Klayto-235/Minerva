#include "mnpch.h"
#include "Minerva/Renderer/Renderer2D.h"
#include "Minerva/Renderer/RenderCommand.h"

#include <glm/gtc/matrix_transform.hpp>


namespace Minerva
{

	Renderer2D::Renderer2D()
	{
		MN_PROFILE_FUNCTION();

		m_quadVertexArray = VertexArray::create();

		m_quadVertexBuffer = VertexBuffer::create(sc_maxVertices*sizeof(QuadVertex));
		m_quadVertexBuffer->setLayout({
			{ ShaderDataType::Float3, "a_position" },
			{ ShaderDataType::Float4, "a_color" },
			{ ShaderDataType::Float2, "a_texCoord" },
			{ ShaderDataType::Float, "a_texIndex" },
			{ ShaderDataType::Float, "a_tilingFactor" }
		});
		m_quadVertexArray->addVertexBuffer(m_quadVertexBuffer);

		m_quadVertexBufferStage = Scope<QuadVertex[]>(new QuadVertex[sc_maxQuads]);

		uint32_t* quadIndices = new uint32_t[sc_maxIndices];
		for (uint32_t i = 0, offset = 0; i < sc_maxIndices; i += 6, offset += 4)
		{
			quadIndices[i + 0] = offset + 0;
			quadIndices[i + 1] = offset + 1;
			quadIndices[i + 2] = offset + 2;
			quadIndices[i + 3] = offset + 2;
			quadIndices[i + 4] = offset + 3;
			quadIndices[i + 5] = offset + 0;
		}
		Ref<IndexBuffer> quadIndexBuffer(IndexBuffer::create(quadIndices, sc_maxIndices));
		delete[] quadIndices;
		m_quadVertexArray->setIndexBuffer(quadIndexBuffer);

		uint32_t data = 0xffffffff;
		m_whiteTexture = Texture2D::create(1, 1);
		m_whiteTexture->setData(&data, sizeof(uint32_t));

		int32_t samplers[sc_maxTextureSlots];
		for (uint32_t i = 0; i < sc_maxTextureSlots; ++i)
			samplers[i] = i;

		m_textureShader = Minerva::Shader::create("assets/shaders/texture.glsl");
		m_textureShader->bind();
		m_textureShader->setIntArray("u_textures", samplers, sc_maxTextureSlots);

		m_textureSlots[0] = m_whiteTexture;
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

		m_quadVertexBufferPtr = m_quadVertexBufferStage.get();
		m_quadVertexBufferStageQuadCount = 0;

		m_textureSlotCount = 1;
	}

	void Renderer2D::endScene()
	{
		MN_PROFILE_FUNCTION();

		flush();
	}

	void Renderer2D::flush()
	{
		MN_PROFILE_FUNCTION();

		if (m_quadVertexBufferStageQuadCount > 0)
		{
			m_quadVertexBuffer->setData(m_quadVertexBufferStage.get(), 4*sizeof(QuadVertex)*m_quadVertexBufferStageQuadCount);

			for (uint32_t i = 0; i < m_textureSlotCount; i++)
				m_textureSlots[i]->bind(i);

			m_quadVertexArray->bind();
			RenderCommand::drawIndexed(m_quadVertexArray, 0, 6*m_quadVertexBufferStageQuadCount);

			m_quadVertexBufferPtr = m_quadVertexBufferStage.get();
			m_quadVertexBufferStageQuadCount = 0;

			m_textureSlotCount = 1;
		}
	}

	void Renderer2D::drawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		MN_PROFILE_FUNCTION();

		if (m_quadVertexBufferStageQuadCount == sc_maxQuads)
			flush();

		m_quadVertexBufferPtr->position = position;
		m_quadVertexBufferPtr->color = color;
		m_quadVertexBufferPtr->texCoord = { 0.0f, 0.0f };
		m_quadVertexBufferPtr->texIndex = 0.0f;
		m_quadVertexBufferPtr->tilingFactor = 1.0f;
		++m_quadVertexBufferPtr;
		
		m_quadVertexBufferPtr->position = { position.x + size.x, position.y, position.z };
		m_quadVertexBufferPtr->color = color;
		m_quadVertexBufferPtr->texCoord = { 1.0f, 0.0f };
		m_quadVertexBufferPtr->texIndex = 0.0f;
		m_quadVertexBufferPtr->tilingFactor = 1.0f;
		++m_quadVertexBufferPtr;
		
		m_quadVertexBufferPtr->position = { position.x + size.x, position.y + size.y, position.z };
		m_quadVertexBufferPtr->color = color;
		m_quadVertexBufferPtr->texCoord = { 1.0f, 1.0f };
		m_quadVertexBufferPtr->texIndex = 0.0f;
		m_quadVertexBufferPtr->tilingFactor = 1.0f;
		++m_quadVertexBufferPtr;
		
		m_quadVertexBufferPtr->position = { position.x, position.y + size.y, position.z };
		m_quadVertexBufferPtr->color = color;
		m_quadVertexBufferPtr->texCoord = { 0.0f, 1.0f };
		m_quadVertexBufferPtr->texIndex = 0.0f;
		m_quadVertexBufferPtr->tilingFactor = 1.0f;
		++m_quadVertexBufferPtr;

		++m_quadVertexBufferStageQuadCount;

		/*
		m_textureShader->bind();
		m_textureShader->setFloat4("u_color", color);

		const glm::mat4 transform = glm::scale(
			glm::translate(glm::mat4(1.0f), position), { size.x, size.y, 1.0f });
		m_textureShader->setMat4("u_M", transform);

		m_whiteTexture->bind();

		m_squareVertexArray->bind();
		RenderCommand::drawIndexed(m_squareVertexArray);
		*/
	}

	void Renderer2D::drawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		drawQuad({ position.x, position.y, 0.0f }, size, color);
	}

	void Renderer2D::drawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture,
		float tilingFactor, const glm::vec4& tintColor)
	{
		MN_PROFILE_FUNCTION();

		float textureIndex = 0.0f;
		for (uint32_t i = 1; i < m_textureSlotCount; i++)
		{
			if (*m_textureSlots[i].get() == *texture.get())
			{
				textureIndex = (float)i;
				break;
			}
		}

		if (m_quadVertexBufferStageQuadCount == sc_maxQuads ||
			(textureIndex == 0.0f && m_textureSlotCount == sc_maxTextureSlots))
			flush();

		if (textureIndex == 0.0f)
		{
			textureIndex = (float)m_textureSlotCount;
			m_textureSlots[m_textureSlotCount] = texture;
			++m_textureSlotCount;
		}

		m_quadVertexBufferPtr->position = position;
		m_quadVertexBufferPtr->color = tintColor;
		m_quadVertexBufferPtr->texCoord = { 0.0f, 0.0f };
		m_quadVertexBufferPtr->texIndex = textureIndex;
		m_quadVertexBufferPtr->tilingFactor = tilingFactor;
		++m_quadVertexBufferPtr;

		m_quadVertexBufferPtr->position = { position.x + size.x, position.y, position.z };
		m_quadVertexBufferPtr->color = tintColor;
		m_quadVertexBufferPtr->texCoord = { 1.0f, 0.0f };
		m_quadVertexBufferPtr->texIndex = textureIndex;
		m_quadVertexBufferPtr->tilingFactor = tilingFactor;
		++m_quadVertexBufferPtr;

		m_quadVertexBufferPtr->position = { position.x + size.x, position.y + size.y, position.z };
		m_quadVertexBufferPtr->color = tintColor;
		m_quadVertexBufferPtr->texCoord = { 1.0f, 1.0f };
		m_quadVertexBufferPtr->texIndex = textureIndex;
		m_quadVertexBufferPtr->tilingFactor = tilingFactor;
		++m_quadVertexBufferPtr;

		m_quadVertexBufferPtr->position = { position.x, position.y + size.y, position.z };
		m_quadVertexBufferPtr->color = tintColor;
		m_quadVertexBufferPtr->texCoord = { 0.0f, 1.0f };
		m_quadVertexBufferPtr->texIndex = textureIndex;
		m_quadVertexBufferPtr->tilingFactor = tilingFactor;
		++m_quadVertexBufferPtr;

		++m_quadVertexBufferStageQuadCount;

		/*
		m_textureShader->bind();
		m_textureShader->setFloat4("u_color", tintColor);
		m_textureShader->setFloat("u_tilingFactor", tilingFactor);

		const glm::mat4 transform = glm::scale(
			glm::translate(glm::mat4(1.0f), position), { size.x, size.y, 1.0f });
		m_textureShader->setMat4("u_M", transform);

		texture->bind();

		m_quadVertexArray->bind();
		RenderCommand::drawIndexed(m_quadVertexArray);
		*/
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

		m_quadVertexArray->bind();
		RenderCommand::drawIndexed(m_quadVertexArray);
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

		m_quadVertexArray->bind();
		RenderCommand::drawIndexed(m_quadVertexArray);
	}

	void Renderer2D::drawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float angle, const Ref<Texture2D>& texture,
		float tilingFactor, const glm::vec4& tintColor)
	{
		drawRotatedQuad({ position.x, position.y, 0.0f }, size, angle, texture, tilingFactor, tintColor);
	}

}