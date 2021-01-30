#include <Minerva.h>

#include "EditorLayer.h"

#include <imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


namespace Minerva
{

	EditorLayer::EditorLayer()
		: Layer("EditorLayer")//, m_cameraController(1280.0f / 720.0f)
	{
		MN_PROFILE_FUNCTION();
	}

	void EditorLayer::onAttach()
	{
		MN_PROFILE_FUNCTION();

		// Framebuffer
		FramebufferProperties framebufferProperties;
		framebufferProperties.width = 1280;
		framebufferProperties.height = 720;
		m_framebuffer = Framebuffer::create(framebufferProperties);

		// Texture
		m_chessboardTexture = Texture2D::create("assets/textures/chess_board.png");

		// Scene & Camera
		m_activeScene = createRef<Scene>();
		m_camera = m_activeScene->newEntity();
		m_camera.addComponent<TransformComponent>();
		auto& cameraComponent = m_camera.addComponent<CameraComponent>();
		cameraComponent.camera.setAspectRatio(1280.0f / 720.0f);
		m_activeScene->setMainCamera(&m_camera);

		// My quad
		auto quad = m_activeScene->newEntity("My quad");
		quad.addComponent<Transform2DComponent>();
		quad.addComponent<SpriteRenderComponent>(glm::vec4{ 0.0f, 1.0f, 0.0f, 1.0f });
		m_quadEntity = quad;
	}

	void EditorLayer::onDetach()
	{
		MN_PROFILE_FUNCTION();
	}

	void EditorLayer::onUpdate(const float timeStep, const InputState& inputState)
	{
		MN_PROFILE_FUNCTION();

		const FramebufferProperties& framebufferProperties = m_framebuffer->getProperties();
		if ((framebufferProperties.width != m_viewportWindowSize.first
			|| framebufferProperties.height != m_viewportWindowSize.second)
			&& m_viewportWindowSize.first != 0 && m_viewportWindowSize.second != 0)
		{
			m_framebuffer->resize(m_viewportWindowSize.first, m_viewportWindowSize.second);
			m_activeScene->onViewportResize(m_viewportWindowSize.first, m_viewportWindowSize.second);
		}

		m_activeScene->onUpdate(timeStep, inputState);
	}

	void EditorLayer::onRender(Renderer2D& renderer2D)
	{
		MN_PROFILE_FUNCTION();

		m_framebuffer->bind();

		RenderCommand::setClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		RenderCommand::clear();

		m_activeScene->onRender(renderer2D);

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
			auto viewportWindowSize = ImGui::GetContentRegionAvail();
			m_viewportWindowSize = { (uint32_t)viewportWindowSize.x , (uint32_t)viewportWindowSize.y };
			uint32_t textureID = m_framebuffer->getColorAttachmentRenderID();
			ImGui::Image(reinterpret_cast<void*>((uint64_t)textureID), viewportWindowSize, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
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
			if (m_quadEntity)
			{
				ImGui::Separator();
				auto& name = m_quadEntity.getComponents<TagComponent>().name;
				ImGui::Text("%s", name.c_str());
				auto& color = m_quadEntity.getComponents<SpriteRenderComponent>().color;
				ImGui::ColorEdit4("Color", glm::value_ptr(color));
			}

			ImGui::DragFloat3("Camera position", glm::value_ptr(m_camera.getComponents<TransformComponent>().matrix[3]));
		}
		ImGui::End();
	}

	bool EditorLayer::onEvent(const Event& event)
	{
		MN_PROFILE_FUNCTION();

		return false;
	}

}