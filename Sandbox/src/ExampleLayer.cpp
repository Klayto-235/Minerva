#include "ExampleLayer.h"

#include <imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


ExampleLayer::ExampleLayer()
	: Layer("Example"), m_cameraController(1280.0f / 720.0f, true)
{
	m_vertexArray = Minerva::VertexArray::create();

	float vertices[3 * 7] = {
		-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
			0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
			0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
	};

	Minerva::Ref<Minerva::VertexBuffer> vertexBuffer;
	vertexBuffer = Minerva::VertexBuffer::create(vertices, sizeof(vertices) / sizeof(vertices[0]));
	vertexBuffer->setLayout({
		{ Minerva::ShaderDataType::Float3, "a_position" },
		{ Minerva::ShaderDataType::Float4, "a_color" }
		});
	m_vertexArray->addVertexBuffer(vertexBuffer);

	uint32_t indices[3] = { 0, 1, 2 };

	Minerva::Ref<Minerva::IndexBuffer> indexBuffer;
	indexBuffer = Minerva::IndexBuffer::create(indices, sizeof(indices) / sizeof(indices[0]));
	m_vertexArray->setIndexBuffer(indexBuffer);

	m_squareVA = Minerva::VertexArray::create();

	float squareVertices[5 * 4] = {
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
		-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
	};

	Minerva::Ref<Minerva::VertexBuffer> squareVB;
	squareVB = Minerva::VertexBuffer::create(squareVertices, sizeof(squareVertices));
	squareVB->setLayout({
		{ Minerva::ShaderDataType::Float3, "a_position" },
		{ Minerva::ShaderDataType::Float2, "a_texCoord" }
		});
	m_squareVA->addVertexBuffer(squareVB);

	uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
	Minerva::Ref<Minerva::IndexBuffer> squareIB;
	squareIB = Minerva::IndexBuffer::create(squareIndices, sizeof(squareIndices) / sizeof(squareIndices[0]));
	m_squareVA->setIndexBuffer(squareIB);

	std::string vertexSrc = R"(
		#version 330 core
			
		layout(location = 0) in vec3 a_position;
		layout(location = 1) in vec4 a_color;

		uniform mat4 u_VP;
		uniform mat4 u_M;

		out vec3 v_position;
		out vec4 v_color;

		void main()
		{
			v_position = a_position;
			v_color = a_color;
			gl_Position = u_VP * u_M * vec4(a_position, 1.0);	
		}
	)";

	std::string fragmentSrc = R"(
		#version 330 core
			
		layout(location = 0) out vec4 color;

		in vec3 v_position;
		in vec4 v_color;

		void main()
		{
			color = v_color;
		}
	)";

	m_shader = Minerva::Shader::create(vertexSrc, fragmentSrc);

	m_flatColorShader = Minerva::Shader::create("assets/shaders/flat_color.glsl");

	auto textureShader = m_shaderLibrary.load("assets/shaders/texture.glsl");

	m_texture = Minerva::Texture2D::create("assets/textures/chess_board.png");
	m_textureAlpha = Minerva::Texture2D::create("assets/textures/alpha_test.png");

	textureShader->bind();
	textureShader->setInt("u_texture", 0);
}

bool ExampleLayer::onEvent(const Minerva::Event& event)
{
	return m_cameraController.onEvent(event);
}

void ExampleLayer::onRender(Minerva::Renderer2D& renderer2D)
{
	Minerva::RenderCommand::setClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
	Minerva::RenderCommand::clear();

	Minerva::Renderer::beginScene(m_cameraController.getCamera());

	glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

	m_flatColorShader->bind();
	m_flatColorShader->setFloat4("u_color", m_squareColor);

	for (int y = 0; y < 20; y++)
	{
		for (int x = 0; x < 20; x++)
		{
			glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
			glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
			Minerva::Renderer::submit(m_flatColorShader, m_squareVA, transform);
		}
	}

	auto textureShader = m_shaderLibrary.get("texture");
	m_texture->bind();
	Minerva::Renderer::submit(textureShader, m_squareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));
	m_textureAlpha->bind();
	Minerva::Renderer::submit(textureShader, m_squareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

	// Triangle
	//Minerva::Renderer::submit(m_shader, m_vertexArray, glm::mat4(1.0f));

	Minerva::Renderer::endScene();
}

void ExampleLayer::onImGuiRender()
{
	ImGui::Begin("Settings");
	ImGui::ColorEdit3("Square Color", glm::value_ptr(m_squareColor));
	ImGui::End();
}