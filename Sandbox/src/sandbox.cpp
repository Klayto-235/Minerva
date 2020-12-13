#include <Minerva.h>
#include <imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Platform/OpenGL/OpenGLShader.h" // TEMPORARY


class ExampleLayer : public Minerva::Layer
{
public:
	ExampleLayer()
		: Layer("Example"), m_camera(3.2f, 1.8f, 2.0f)
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

		std::string flatColorShaderVertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_position;

			uniform mat4 u_VP;
			uniform mat4 u_M;

			out vec3 v_position;

			void main()
			{
				v_position = a_position;
				gl_Position = u_VP * u_M * vec4(a_position, 1.0);	
			}
		)";

		std::string flatColorShaderFragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			uniform vec3 u_color;

			in vec3 v_position;

			void main()
			{
				color = vec4(u_color, 1.0);
			}
		)";

		m_flatColorShader = Minerva::Shader::create(flatColorShaderVertexSrc, flatColorShaderFragmentSrc);

		auto textureShader = m_shaderLibrary.load("assets/shaders/.");

		m_texture = Minerva::Texture2D::create("assets/textures/chess_board.png");
		m_textureAlpha = Minerva::Texture2D::create("assets/textures/alpha_test.png");

		std::dynamic_pointer_cast<Minerva::OpenGLShader>(textureShader)->bind();
		std::dynamic_pointer_cast<Minerva::OpenGLShader>(textureShader)->uploadUniformInt("u_texture", 0);
	}

	void onUpdate(float ts) override
	{
		const Minerva::Window& window = Minerva::Application::get().getWindow();

		if (window.isKeyPressed(Minerva::Key::Left))
			m_cameraPosition.x -= m_cameraMoveSpeed * ts;
		else if (window.isKeyPressed(Minerva::Key::Right))
			m_cameraPosition.x += m_cameraMoveSpeed * ts;

		if (window.isKeyPressed(Minerva::Key::Up))
			m_cameraPosition.y += m_cameraMoveSpeed * ts;
		else if (window.isKeyPressed(Minerva::Key::Down))
			m_cameraPosition.y -= m_cameraMoveSpeed * ts;

		if (window.isKeyPressed(Minerva::Key::A))
			m_cameraRotation += m_cameraRotationSpeed * ts;
		if (window.isKeyPressed(Minerva::Key::D))
			m_cameraRotation -= m_cameraRotationSpeed * ts;

		m_camera.setPosition(m_cameraPosition);
		m_camera.setRotation(m_cameraRotation);

		Minerva::RenderCommand::setClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		Minerva::RenderCommand::clear();

		Minerva::Renderer::beginScene(m_camera);

		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

		std::dynamic_pointer_cast<Minerva::OpenGLShader>(m_flatColorShader)->bind();
		std::dynamic_pointer_cast<Minerva::OpenGLShader>(m_flatColorShader)->
			uploadUniformFloat3("u_color", m_squareColor);

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

	bool onEvent(const Minerva::Event& event) override
	{
		return false;
	}

	void onImGuiRender() override
	{
		ImGui::Begin("Settings");
		ImGui::ColorEdit3("Square Color", glm::value_ptr(m_squareColor));
		ImGui::End();
	}

private:
	Minerva::ShaderLibrary m_shaderLibrary;
	Minerva::Ref<Minerva::Shader> m_shader;
	Minerva::Ref<Minerva::VertexArray> m_vertexArray;

	Minerva::Ref<Minerva::Shader> m_flatColorShader;
	Minerva::Ref<Minerva::VertexArray> m_squareVA;

	Minerva::Ref<Minerva::Texture2D> m_texture;
	Minerva::Ref<Minerva::Texture2D> m_textureAlpha;

	Minerva::OrthographicCamera m_camera;
	glm::vec3 m_cameraPosition = { 0.0f, 0.0f, 0.0f };
	float m_cameraMoveSpeed = 5.0f;
	float m_cameraRotation = 0.0f;
	float m_cameraRotationSpeed = 180.0f;

	glm::vec3 m_squareColor = { 0.2f, 0.3f, 0.8f };
};

class Sandbox : public Minerva::Application
{
public:
	Sandbox()
	{
		pushLayer(new ExampleLayer());
	}

	~Sandbox()
	{

	}
};

Minerva::Application* Minerva::createApplication()
{
	return new Sandbox();
}