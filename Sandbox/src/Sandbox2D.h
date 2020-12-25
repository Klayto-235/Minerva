#pragma once

#include <Minerva.h>


class Sandbox2D : public Minerva::Layer
{
public:
	Sandbox2D();

	void onAttach() override;
	void onDetach() override;
	bool onEvent(const Minerva::Event& event) override;
	void onUpdate(const float timeStep, const Minerva::WindowInputState& inputState) override;
	void onRender(Minerva::Renderer2D& renderer2D) override;
	void onImGuiRender() override;
private:
	Minerva::OrthographicCameraController m_cameraController;

	glm::vec4 m_quadColor = { 0.2f, 0.3f, 0.8f, 1.0f };
};