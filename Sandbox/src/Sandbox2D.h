#pragma once

#include <Minerva.h>


class Sandbox2D : public Minerva::Layer
{
public:
	Sandbox2D();

	void onAttach() override;
	void onDetach() override;
	void onUpdate(const float timeStep, const Minerva::WindowInputState& inputState) override;
	void onImGuiRender() override;
	bool onEvent(const Minerva::Event& event) override;
private:
	Minerva::OrthographicCameraController m_cameraController;

	// TEMPORARY
	Minerva::Ref<Minerva::VertexArray> m_squareVA;
	Minerva::Ref<Minerva::Shader> m_flatColorShader;

	glm::vec4 m_squareColor = { 0.2f, 0.3f, 0.8f, 1.0f };
};