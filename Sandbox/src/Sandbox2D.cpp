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
	Minerva::Renderer::init();

	m_squareVA = Minerva::VertexArray::create();

	float squareVertices[3 * 4] = {
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.5f,  0.5f, 0.0f,
		-0.5f,  0.5f, 0.0f
	};

	Minerva::Ref<Minerva::VertexBuffer> squareVB;
	squareVB = Minerva::VertexBuffer::create(squareVertices, sizeof(squareVertices));
	squareVB->setLayout({
		{ Minerva::ShaderDataType::Float3, "a_position" }
		});
	m_squareVA->addVertexBuffer(squareVB);

	uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
	Minerva::Ref<Minerva::IndexBuffer> squareIB;
	squareIB = Minerva::IndexBuffer::create(squareIndices, sizeof(squareIndices) / sizeof(squareIndices[0]));
	m_squareVA->setIndexBuffer(squareIB);

	m_flatColorShader = Minerva::Shader::create("assets/shaders/flat_color.glsl");
}

void Sandbox2D::onDetach()
{
}

void Sandbox2D::onUpdate(const float timeStep, const Minerva::WindowInputState& inputState)
{
	// Update
	m_cameraController.onUpdate(timeStep, inputState);

	// Render
	Minerva::RenderCommand::setClearColor({ 0.1f, 0.1f, 0.1f, 1 });
	Minerva::RenderCommand::clear();

	Minerva::Renderer::beginScene(m_cameraController.getCamera());

	std::dynamic_pointer_cast<Minerva::OpenGLShader>(m_flatColorShader)->bind();
	std::dynamic_pointer_cast<Minerva::OpenGLShader>(m_flatColorShader)->
		uploadUniformFloat4("u_color", m_squareColor);

	Minerva::Renderer::submit(m_flatColorShader, m_squareVA,
		glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

	Minerva::Renderer::endScene();
}

void Sandbox2D::onImGuiRender()
{
	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_squareColor));
	ImGui::End();
}

bool Sandbox2D::onEvent(const Minerva::Event& event)
{
	return m_cameraController.onEvent(event);
}
