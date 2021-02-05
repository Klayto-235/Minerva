#pragma once

#include "Panels/SceneHierarchyPanel.h"
#include "Panels/PropertiesPanel.h"

#include <Minerva.h>

#include <imgui.h>


namespace Minerva
{
	struct EditorContext
	{
		enum class Type { None = 0, Scene = 1, Entity = 2 };

		Type type = Type::None;
		void* ptr = nullptr;

		bool operator==(const EditorContext& other) { return type == other.type && ptr == other.ptr; }
		bool operator!=(const EditorContext& other) { return type != other.type || ptr != other.ptr; }
	};

	struct EditorState
	{
		// scene is initialised so it doesn't compare equal to selection
		EditorContext scene{ EditorContext::Type::Scene, nullptr };
		EditorContext selection{};
	};

	class EditorLayer : public Layer
	{
	public:
		EditorLayer();

		void onAttach() override;
		void onDetach() override;
		bool onEvent(const Event& event) override;
		void onUpdate(const float timeStep, const InputState& inputState) override;
		void onRender(Renderer2D& renderer2D) override;
		void onImGuiRender() override;
	private:
		EditorState m_state{};

		SceneHierarchyPanel m_sceneHierarchyPanel;
		PropertiesPanel m_propertiesPanel;

		std::pair<uint32_t, uint32_t> m_viewportWindowSize = { 0, 0 };
		bool m_overrideBlockEvents = false;

		Ref<Framebuffer> m_framebuffer;

		Ref<Texture2D> m_chessboardTexture;

		Ref<Scene> m_activeScene;
		Entity m_quadEntity{};
		Entity m_camera{};

		glm::vec4 m_quadColor = { 0.2f, 0.3f, 0.8f, 1.0f };
		float m_quadRotation = 0.0f;

#if defined MN_ENABLE_DEBUG_CODE
		Renderer2D::Statistics m_renderer2DStatistics;
#endif
	};

}