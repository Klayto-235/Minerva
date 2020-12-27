#pragma once

#include <Minerva.h>


class ExampleLayer : public Minerva::Layer
{
public:
	ExampleLayer();

	bool onEvent(const Minerva::Event& event) override;
	void onRender(Minerva::Renderer2D& renderer2D) override;
	void onImGuiRender() override;

private:
	Minerva::ShaderLibrary m_shaderLibrary;
	Minerva::Ref<Minerva::Shader> m_shader;
	Minerva::Ref<Minerva::VertexArray> m_vertexArray;

	Minerva::Ref<Minerva::Shader> m_flatColorShader;
	Minerva::Ref<Minerva::VertexArray> m_squareVA;

	Minerva::Ref<Minerva::Texture2D> m_texture;
	Minerva::Ref<Minerva::Texture2D> m_textureAlpha;

	Minerva::OrthographicCameraController m_cameraController;

	glm::vec4 m_squareColor = { 0.2f, 0.3f, 0.8f, 1.0f };
};