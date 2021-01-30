#pragma once

#include "Minerva/core/core.h"
#include "Minerva/Renderer/Buffer.h"
#include "Minerva/Renderer/Shader.h"
#include "Minerva/Renderer/Texture.h"
#include "Minerva/Renderer/Camera.h"

#include <glm/glm.hpp>


namespace Minerva
{

	class Renderer2D
	{
	public:
		Renderer2D();
		~Renderer2D();

		void beginScene(const Camera& camera, const glm::mat4& viewMatrix);
		void beginScene(const OrthographicCamera& camera);
		void endScene();
		void flush();

		void drawQuad(const glm::mat3& transform, const glm::vec4& color, float z = 0.0f);
		void drawQuad(const glm::mat3& transform, const Ref<Texture2D>& texture,
			float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f), float z = 0.0f);
		void drawQuad(const glm::mat4& transform, const glm::vec4& color);
		void drawQuad(const glm::mat4& transform, const Ref<Texture2D>& texture,
			float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));

		void drawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
		void drawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
		void drawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture,
			float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));
		void drawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture,
			float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));

		void drawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float angle, const glm::vec4& color);
		void drawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float angle, const glm::vec4& color);
		void drawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float angle,
			const Ref<Texture2D>& texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));
		void drawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float angle,
			const Ref<Texture2D>& texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));
	
#if defined MN_ENABLE_DEBUG_CODE
		struct Statistics
		{
			uint32_t nDrawCalls = 0;
			uint32_t nQuads = 0;

			uint32_t getVertexCount() { return 4*nQuads; }
			uint32_t getIndexCount() { return 6*nQuads; }
		};
		void resetStatistics() { m_statistics = {}; }
		const Statistics& getStatistics() const { return m_statistics; }
	private:
		Statistics m_statistics;
#endif
	private:
		struct QuadVertex
		{
			glm::vec3 position;
			glm::vec4 color;
			glm::vec2 texCoord;
			float texIndex;
			float tilingFactor;
		};

		static constexpr uint32_t sc_maxQuads = 10000;
		static constexpr uint32_t sc_maxVertices = 4*sc_maxQuads;
		static constexpr uint32_t sc_maxIndices = 6*sc_maxQuads;
		static constexpr uint32_t sc_maxTextureSlots = 16;

		static constexpr glm::vec4 sc_quadVertexPositionsVec4[4] = {
			{ -0.5f, -0.5f, 0.0f, 1.0f },
			{  0.5f, -0.5f, 0.0f, 1.0f },
			{  0.5f,  0.5f, 0.0f, 1.0f },
			{ -0.5f,  0.5f, 0.0f, 1.0f }
		};
		static constexpr glm::vec3 sc_quadVertexPositionsVec3[4] = {
			{ -0.5f, -0.5f, 1.0f },
			{  0.5f, -0.5f, 1.0f },
			{  0.5f,  0.5f, 1.0f },
			{ -0.5f,  0.5f, 1.0f }
		};
		static constexpr glm::vec2 sc_quadVertexTexCoords[4] = {
			{ 0.0f, 0.0f },
			{ 1.0f, 0.0f },
			{ 1.0f, 1.0f },
			{ 0.0f, 1.0f }
		};

		Ref<VertexArray> m_quadVertexArray;
		Ref<VertexBuffer> m_quadVertexBuffer;
		Ref<Shader> m_textureShader;

		Scope<QuadVertex[]> m_quadVertexBufferStage;
		QuadVertex* m_quadVertexBufferPtr = nullptr;
		uint32_t m_quadVertexBufferStageQuadCount = 0;

		std::array<Ref<Texture2D>, sc_maxTextureSlots> m_textureSlots;
		uint32_t m_textureSlotCount = 0;
	};

}