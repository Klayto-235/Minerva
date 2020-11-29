#include "mnpch.h"
#include "Application.h"
#include "Log.h"

// TEMPORARY
#include <glad/glad.h>


namespace Minerva
{

	Application* Application::s_instance = nullptr;

	Application::Application()
		: m_running(false)
	{
		MN_CORE_ASSERT(!s_instance, "application already exists.");
		s_instance = this;

		m_window = std::unique_ptr<Window>(Window::create());

		m_ImGuiLayer = new ImGuiLayer();
		m_layerStack.pushOverlay(m_ImGuiLayer);

		// TEMPORARY
		glGenVertexArrays(1, &m_vertexArray);
		glBindVertexArray(m_vertexArray);

		glGenBuffers(1, &m_vertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);

		float vertices[3 * 3] = {
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.0f,  0.5f, 0.0f
		};

		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

		glGenBuffers(1, &m_indexBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer);

		unsigned int indices[3] = { 0, 1, 2 };
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			out vec3 v_Position;
			void main()
			{
				v_Position = a_Position;
				gl_Position = vec4(a_Position, 1.0);	
			}
		)";

		std::string fragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;
			in vec3 v_Position;
			void main()
			{
				color = vec4(v_Position * 0.5 + 0.5, 1.0);
			}
		)";

		m_shader.reset(new Shader(vertexSrc, fragmentSrc));
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
			glClearColor(0.1f, 0.1f, 0.1f, 1);
			glClear(GL_COLOR_BUFFER_BIT);

			// TEMPORARY
			m_shader->bind();
			glBindVertexArray(m_vertexArray);
			glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);

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