#pragma once

#include "core.h"
#include "Window.h"
#include "LayerStack.h"
#include "ImGui/ImGuiLayer.h"

#include "Renderer/Shader.h"
#include "Renderer/Buffer.h"
#include "Renderer/Camera.h"


namespace Minerva
{

	class  Application
	{
	public:
		virtual ~Application();
		Application(const Application& other) = delete;
		Application& operator=(const Application& other) = delete;

		void run();

		void pushLayer(Layer* layer);
		void pushOverlay(Layer* overlay);

		Window& getWindow() { return *m_window; }

		static Application& get() { return *s_instance; }
	protected:
		Application();
	private:
		static Application* s_instance;

		std::unique_ptr<Window> m_window;
		bool m_running;
		LayerStack m_layerStack;
		ImGuiLayer* m_ImGuiLayer;

		std::shared_ptr<Shader> m_shader;
		std::shared_ptr<VertexArray> m_vertexArray;

		std::shared_ptr<Shader> m_blueShader;
		std::shared_ptr<VertexArray> m_squareVA;

		OrthographicCamera m_camera;
	};

	// To be defined in CLIENT
	Application* createApplication();

}