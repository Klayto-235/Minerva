#include "EditorLayer.h"

#include <imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


namespace Minerva
{

	EditorLayer::EditorLayer()
		: Layer("EditorLayer"), m_sceneHierarchyPanel(&m_state), m_propertiesPanel(&m_state)
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

		// Scene
		m_activeScene = createRef<Scene>();

		class CameraController : public NativeScriptBase
		{
		public:
			void onUpdate(float timeStep, const InputState& inputState) override
			{
				auto& translation = getComponents<TransformComponent>().translation;
				constexpr float speed = 2.0f;

				if (inputState.isKeyPressed(Key::A))
					translation.x -= speed * timeStep;
				if (inputState.isKeyPressed(Key::D))
					translation.x += speed * timeStep;
				if (inputState.isKeyPressed(Key::W))
					translation.y += speed * timeStep;
				if (inputState.isKeyPressed(Key::S))
					translation.y -= speed * timeStep;
			}
		};
		auto camera = m_activeScene->newEntity("Camera 1");
		camera.addComponent<TransformComponent>();
		auto& cameraComponent = camera.addComponent<CameraComponent>();
		cameraComponent.camera.setAspectRatio(1280.0f / 720.0f);
		cameraComponent.camera.setProjectionType(SceneCamera::ProjectionType::Orthographic);
		camera.addNativeScript<CameraController>();
		m_activeScene->setMainCamera(camera);

		auto camera2 = m_activeScene->newEntity("Camera 2");
		camera2.addComponent<TransformComponent>();
		auto& cameraComponent2 = camera2.addComponent<CameraComponent>();
		cameraComponent2.camera.setAspectRatio(1280.0f / 720.0f);
		cameraComponent2.camera.setProjectionType(SceneCamera::ProjectionType::Orthographic);

		auto redSquare = m_activeScene->newEntity("Red square");
		redSquare.addComponent<TransformComponent>().translation.x = 0.2f;
		redSquare.addComponent<SpriteRenderComponent>(glm::vec4{ 1.0f, 0.0f, 0.0f, 1.0f });

		auto quad = m_activeScene->newEntity("My quad");
		quad.addComponent<TransformComponent>();
		quad.addComponent<SpriteRenderComponent>(glm::vec4{ 0.0f, 1.0f, 0.0f, 1.0f });

		m_state.scene = { EditorContext::Type::Scene, m_activeScene.get() };
		m_state.selection = m_state.scene;

		m_activeScene->onStart();
	}

	void EditorLayer::onDetach()
	{
		MN_PROFILE_FUNCTION();

		m_activeScene->onStop();
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

		m_sceneHierarchyPanel.onImGuiRender();
		m_propertiesPanel.onImGuiRender();

		if (ImGui::Begin("Statistics"))
		{
#if defined MN_ENABLE_DEBUG_CODE
			ImGui::Text("Renderer2D Stats:");
			ImGui::Text("Draw Calls: %d", m_renderer2DStatistics.nDrawCalls);
			ImGui::Text("Quads: %d", m_renderer2DStatistics.nQuads);
			ImGui::Text("Vertices: %d", m_renderer2DStatistics.getVertexCount());
			ImGui::Text("Indices: %d", m_renderer2DStatistics.getIndexCount());
#endif
		}
		ImGui::End();
	}

	bool EditorLayer::onEvent(const Event& event)
	{
		MN_PROFILE_FUNCTION();

		return false;
	}

}