#include <Minerva.h>
#include <imgui.h>

class ExampleLayer : public Minerva::Layer
{
public:
	ExampleLayer()
		: Layer("Example"), m_camera(3.2f, 1.8f, 2.0f)
	{
		m_vertexArray.reset(Minerva::VertexArray::create());

		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
		};

		std::shared_ptr<Minerva::VertexBuffer> vertexBuffer;
		vertexBuffer.reset(Minerva::VertexBuffer::create(vertices, sizeof(vertices) / sizeof(vertices[0])));
		vertexBuffer->setLayout({
			{ Minerva::ShaderDataType::Float3, "a_position" },
			{ Minerva::ShaderDataType::Float4, "a_color" }
			});
		m_vertexArray->addVertexBuffer(vertexBuffer);

		uint32_t indices[3] = { 0, 1, 2 };

		std::shared_ptr<Minerva::IndexBuffer> indexBuffer;
		indexBuffer.reset(Minerva::IndexBuffer::create(indices, sizeof(indices) / sizeof(indices[0])));
		m_vertexArray->setIndexBuffer(indexBuffer);

		m_squareVA.reset(Minerva::VertexArray::create());

		float squareVertices[3 * 4] = {
			-0.75f, -0.75f, 0.0f,
			 0.75f, -0.75f, 0.0f,
			 0.75f,  0.75f, 0.0f,
			-0.75f,  0.75f, 0.0f
		};

		std::shared_ptr<Minerva::VertexBuffer> squareVB;
		squareVB.reset(Minerva::VertexBuffer::create(squareVertices, sizeof(squareVertices)));
		squareVB->setLayout({
			{ Minerva::ShaderDataType::Float3, "a_position" }
			});
		m_squareVA->addVertexBuffer(squareVB);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		std::shared_ptr<Minerva::IndexBuffer> squareIB;
		squareIB.reset(Minerva::IndexBuffer::create(squareIndices, sizeof(squareIndices) / sizeof(squareIndices[0])));
		m_squareVA->setIndexBuffer(squareIB);

		std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_position;
			layout(location = 1) in vec4 a_color;

			uniform mat4 u_VP;

			out vec3 v_position;
			out vec4 v_color;

			void main()
			{
				v_position = a_position;
				v_color = a_color;
				gl_Position = u_VP * vec4(a_position, 1.0);	
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

		m_shader.reset(new Minerva::Shader(vertexSrc, fragmentSrc));

		std::string blueShaderVertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_position;

			uniform mat4 u_VP;

			out vec3 v_position;

			void main()
			{
				v_position = a_position;
				gl_Position = u_VP * vec4(a_position, 1.0);	
			}
		)";

		std::string blueShaderFragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec3 v_position;

			void main()
			{
				color = vec4(0.2, 0.3, 0.8, 1.0);
			}
		)";

		m_blueShader.reset(new Minerva::Shader(blueShaderVertexSrc, blueShaderFragmentSrc));
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

		Minerva::Renderer::submit(m_blueShader, m_squareVA);
		Minerva::Renderer::submit(m_shader, m_vertexArray);

		Minerva::Renderer::endScene();
	}

	bool onEvent(const Minerva::Event& event) override
	{
		return false;
	}

	void onImGuiRender() override
	{

	}
private:
	std::shared_ptr<Minerva::Shader> m_shader;
	std::shared_ptr<Minerva::VertexArray> m_vertexArray;

	std::shared_ptr<Minerva::Shader> m_blueShader;
	std::shared_ptr<Minerva::VertexArray> m_squareVA;

	Minerva::OrthographicCamera m_camera;
	glm::vec3 m_cameraPosition = { 0.0f, 0.0f, 0.0f };
	float m_cameraMoveSpeed = 5.0f;
	float m_cameraRotation = 0.0f;
	float m_cameraRotationSpeed = 180.0f;
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