#include "Sandbox2D.h"

#include <imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D"), m_cameraController(1280.0f / 720.0f)
{
	MN_PROFILE_FUNCTION();
}

void Sandbox2D::onAttach()
{
	MN_PROFILE_FUNCTION();

	m_chessboardTexture = Minerva::Texture2D::create("assets/textures/chess_board.png");
}

void Sandbox2D::onDetach()
{
	MN_PROFILE_FUNCTION();
}

void Sandbox2D::onUpdate(const float timeStep, const Minerva::WindowInputState& inputState)
{
	MN_PROFILE_FUNCTION();

	m_cameraController.onUpdate(timeStep, inputState);
}

void Sandbox2D::onRender(Minerva::Renderer2D& renderer2D)
{
	MN_PROFILE_FUNCTION();

	Minerva::RenderCommand::setClearColor({ 0.1f, 0.1f, 0.1f, 1 });
	Minerva::RenderCommand::clear();

	renderer2D.beginScene(m_cameraController.getCamera());
	renderer2D.drawQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, m_quadColor);
	renderer2D.drawQuad({ 0.5f, -0.5f }, { 0.5f, 0.75f }, { 0.2f, 0.3f, 0.8f, 1.0f });
	renderer2D.drawQuad({ 0.0f, 0.0f, -0.1f }, { 10.0f, 10.0f }, m_chessboardTexture);
	renderer2D.endScene();
}

void Sandbox2D::onImGuiRender()
{
	MN_PROFILE_FUNCTION();

	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Quad Color", glm::value_ptr(m_quadColor));
	ImGui::End();
}

bool Sandbox2D::onEvent(const Minerva::Event& event)
{
	MN_PROFILE_FUNCTION();

	return m_cameraController.onEvent(event);
}
