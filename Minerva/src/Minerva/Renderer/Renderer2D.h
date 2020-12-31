#pragma once

#include "Minerva/core/core.h"
#include "Minerva/Renderer/Buffer.h"
#include "Minerva/Renderer/Shader.h"
#include "Minerva/Renderer/Texture.h"
#include "Minerva/Renderer/Camera.h"


namespace Minerva
{

	class Renderer2D
	{
	public:
		Renderer2D();
		~Renderer2D();

		void beginScene(const OrthographicCamera& camera);
		void endScene();

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
	private:
		Ref<VertexArray> m_squareVertexArray;
		Ref<Shader> m_textureShader;
		Ref<Texture2D> m_whiteTexture;
	};

}