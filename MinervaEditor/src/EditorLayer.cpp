#include <Minerva.h>

#include "EditorLayer.h"

#include <imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


namespace Minerva
{

	EditorLayer::EditorLayer()
		: Layer("EditorLayer"), m_cameraController(1280.0f / 720.0f)
	{
		MN_PROFILE_FUNCTION();
	}

	void EditorLayer::onAttach()
	{
		MN_PROFILE_FUNCTION();

		m_chessboardTexture = Texture2D::create("assets/textures/chess_board.png");

		FramebufferProperties framebufferProperties;
		framebufferProperties.width = 1280;
		framebufferProperties.height = 720;
		m_framebuffer = Framebuffer::create(framebufferProperties);
	}

	void EditorLayer::onDetach()
	{
		MN_PROFILE_FUNCTION();
	}

	void EditorLayer::onUpdate(const float timeStep, const WindowInputState& inputState)
	{
		MN_PROFILE_FUNCTION();

		m_cameraController.onUpdate(timeStep, inputState);

		m_quadRotation += timeStep * glm::pi<float>();
	}

	void EditorLayer::onRender(Renderer2D& renderer2D)
	{
		MN_PROFILE_FUNCTION();

		m_framebuffer->bind();

		RenderCommand::setClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		RenderCommand::clear();

		renderer2D.beginScene(m_cameraController.getCamera());
		renderer2D.drawQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, m_quadColor); // doesn't blend well because it's drawn first
		renderer2D.drawQuad({ 0.5f, -0.5f }, { 0.5f, 0.75f }, { 0.2f, 0.3f, 0.8f, 1.0f });
		renderer2D.drawQuad({ 0.0f, 0.0f, -0.1f }, { 8.0f, 8.0f }, m_chessboardTexture, 2.0f);
		renderer2D.drawQuad({ -1.0f, 0.5f, 0.1f }, { 1.0f, 1.0f }, m_chessboardTexture, 0.5f, { 0.9f, 0.95f, 1.0f, 1.0f });
		renderer2D.drawRotatedQuad({ 1.0f, 0.0f, 0.1f }, { 1.0f, 1.0f }, glm::radians(30.0f),
			m_chessboardTexture, 0.5f, { 0.9f, 0.95f, 1.0f, 1.0f });
		renderer2D.drawRotatedQuad({ 1.0f, -0.5f }, { 0.5f, 0.75f }, glm::radians(-15.0f), { 0.5f, 0.5f, 0.5f, 1.0f });
		renderer2D.drawRotatedQuad({ 0.5f, 1.5f, 0.1f }, { 1.0f, 1.0f }, m_quadRotation, m_chessboardTexture);
		renderer2D.endScene();

		renderer2D.beginScene(m_cameraController.getCamera());
		for (float y = -5.0f; y <= 5.0f; y += 0.5f)
		{
			for (float x = -5.0f; x <= 5.0f; x += 0.5f)
			{
				glm::vec4 color = { (x + 5.0f) / 10.0f, 0.4f, (y + 5.0f) / 10.0f, 0.7f };
				renderer2D.drawQuad({ x, y }, { 0.4f, 0.4f }, color);
			}
		}
		renderer2D.endScene();

		m_framebuffer->unbind();

#if defined MN_ENABLE_DEBUG_CODE
		m_renderer2DStatistics = renderer2D.getStatistics();
		renderer2D.resetStatistics();
#endif
	}

	void EditorLayer::onImGuiRender()
	{
		MN_PROFILE_FUNCTION();

		ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				bool exitSelected = false;
				ImGui::MenuItem("Exit", nullptr, &exitSelected);
				if (exitSelected)
					Application::get().stop();
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
		if (ImGui::Begin("Viewport"))
		{
			ImVec2 viewportWindowSize = ImGui::GetContentRegionAvail();
			if (m_viewportSize.x != viewportWindowSize.x || m_viewportSize.y != viewportWindowSize.y)
			{
				m_viewportSize = viewportWindowSize;
				m_framebuffer->resize((uint32_t)viewportWindowSize.x, (uint32_t)viewportWindowSize.y);

				m_cameraController.onEvent(WindowResizeEvent((uint32_t)viewportWindowSize.x, (uint32_t)viewportWindowSize.y));
			}
			uint32_t textureID = m_framebuffer->getColorAttachmentRenderID();
			ImGui::Image(reinterpret_cast<void*>((uint64_t)textureID), m_viewportSize, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
		}

		bool viewportFocused = ImGui::IsWindowFocused();
		bool viewportHovered = ImGui::IsWindowHovered();
		Application::get().setImGuiViewportWindowState(viewportFocused, viewportHovered);

		ImGui::End();
		ImGui::PopStyleVar();

		if (ImGui::Begin("Settings"))
		{
#if defined MN_ENABLE_DEBUG_CODE
			ImGui::Text("Renderer2D Stats:");
			ImGui::Text("Draw Calls: %d", m_renderer2DStatistics.nDrawCalls);
			ImGui::Text("Quads: %d", m_renderer2DStatistics.nQuads);
			ImGui::Text("Vertices: %d", m_renderer2DStatistics.getVertexCount());
			ImGui::Text("Indices: %d", m_renderer2DStatistics.getIndexCount());
#endif

			ImGui::ColorEdit4("Square Color", glm::value_ptr(m_quadColor));
		}
		ImGui::End();

		//bool showDemo = true;
		//ImGui::ShowDemoWindow(&showDemo);
	}

	bool EditorLayer::onEvent(const Event& event)
	{
		MN_PROFILE_FUNCTION();

		return m_cameraController.onEvent(event);
	}

}