#include "Sandbox2D.h"

#include <imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <Platform/OpenGL/OpenGLShader.h>


Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D"), m_cameraController(1280.0f / 720.0f)
{
}

void Sandbox2D::onAttach()
{
}

void Sandbox2D::onDetach()
{
}

void Sandbox2D::onUpdate(const float timeStep, const Minerva::WindowInputState& inputState)
{
	m_cameraController.onUpdate(timeStep, inputState);
}

void Sandbox2D::onRender(Minerva::Renderer2D& renderer2D)
{
	Minerva::RenderCommand::setClearColor({ 0.1f, 0.1f, 0.1f, 1 });
	Minerva::RenderCommand::clear();

	renderer2D.beginScene(m_cameraController.getCamera());
	renderer2D.drawQuad({ 0.0f, 0.0f }, { 1.0f, 1.0f }, m_quadColor);
	renderer2D.endScene();
}

void Sandbox2D::onImGuiRender()
{
	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Quad Color", glm::value_ptr(m_quadColor));
	ImGui::End();
}

bool Sandbox2D::onEvent(const Minerva::Event& event)
{
	return m_cameraController.onEvent(event);
}
