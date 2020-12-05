#include "mnpch.h"
#include "Application.h"
#include "Log.h"
#include "Renderer/Renderer.h"


namespace Minerva
{

	Application* Application::s_instance = nullptr;

	Application::Application()
		: m_running(false), m_camera(3.2f, 1.8f, 2.0f)
	{
		MN_CORE_ASSERT(!s_instance, "application already exists.");
		s_instance = this;

		m_window = std::unique_ptr<Window>(Window::create());

		m_ImGuiLayer = new ImGuiLayer();
		m_layerStack.pushOverlay(m_ImGuiLayer);

		m_vertexArray.reset(VertexArray::create());

		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
		};

		std::shared_ptr<VertexBuffer> vertexBuffer;
		vertexBuffer.reset(VertexBuffer::create(vertices, sizeof(vertices) / sizeof(vertices[0])));
		vertexBuffer->setLayout({
			{ ShaderDataType::Float3, "a_position" },
			{ ShaderDataType::Float4, "a_color" }
		});
		m_vertexArray->addVertexBuffer(vertexBuffer);

		uint32_t indices[3] = { 0, 1, 2 };

		std::shared_ptr<IndexBuffer> indexBuffer;
		indexBuffer.reset(IndexBuffer::create(indices, sizeof(indices) / sizeof(indices[0])));
		m_vertexArray->setIndexBuffer(indexBuffer);

		m_squareVA.reset(VertexArray::create());

		float squareVertices[3 * 4] = {
			-0.75f, -0.75f, 0.0f,
			 0.75f, -0.75f, 0.0f,
			 0.75f,  0.75f, 0.0f,
			-0.75f,  0.75f, 0.0f
		};

		std::shared_ptr<VertexBuffer> squareVB;
		squareVB.reset(VertexBuffer::create(squareVertices, sizeof(squareVertices)));
		squareVB->setLayout({
			{ ShaderDataType::Float3, "a_position" }
		});
		m_squareVA->addVertexBuffer(squareVB);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		std::shared_ptr<IndexBuffer> squareIB;
		squareIB.reset(IndexBuffer::create(squareIndices, sizeof(squareIndices) / sizeof(squareIndices[0])));
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

		m_shader.reset(new Shader(vertexSrc, fragmentSrc));

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

		m_blueShader.reset(new Shader(blueShaderVertexSrc, blueShaderFragmentSrc));
	}

	Application::~Application()
	{
		s_instance = nullptr;
	}

	void Application::run()
	{
		m_running = true;
		while (m_running)
		{
			RenderCommand::setClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
			RenderCommand::clear();

			m_camera.setPosition({ 0.5f, 0.5f, 0.0f });
			m_camera.setRotation(45.0f);

			Renderer::beginScene(m_camera);

			Renderer::submit(m_blueShader, m_squareVA);
			Renderer::submit(m_shader, m_vertexArray);

			Renderer::endScene();

			Window::pollEvents();
			for (auto& event : m_window->getEventBuffer())
			{
				if (event->getEventCategoryFlags() & EventCategory::EventCategoryWindow)
				{
					switch (event->getEventType())
					{
					case EventType::WindowClose:
						m_running = false;
						continue;
					}
				}
				
				for (auto it = m_layerStack.rbegin(); it != m_layerStack.rend(); ++it)
				{
					if ((*it)->onEvent(*event)) break;
				}
			}

			for (Layer* layer : m_layerStack)
				layer->onUpdate();

			m_ImGuiLayer->begin();
			for (Layer* layer : m_layerStack)
				layer->onImGuiRender();
			m_ImGuiLayer->end();

			m_window->onUpdate(); // clears events
		}
	}

	void Application::pushLayer(Layer* layer)
	{
		m_layerStack.pushLayer(layer);
	}

	void Application::pushOverlay(Layer* overlay)
	{
		m_layerStack.pushOverlay(overlay);
	}

}