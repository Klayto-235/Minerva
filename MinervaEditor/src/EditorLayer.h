#pragma once

#include <Minerva/core/Layer.h>

#include <imgui.h>


namespace Minerva
{

	class EditorLayer : public Layer
	{
	public:
		EditorLayer();

		void onAttach() override;
		void onDetach() override;
		bool onEvent(const Event& event) override;
		void onUpdate(const float timeStep, const WindowInputState& inputState) override;
		void onRender(Renderer2D& renderer2D) override;
		void onImGuiRender() override;
	private:
		ImVec2 m_viewportSize = { 0.0f, 0.0f };
		bool m_overrideBlockEvents = false;

		OrthographicCameraController m_cameraController;

		Ref<Framebuffer> m_framebuffer;

		Ref<Texture2D> m_chessboardTexture;

		glm::vec4 m_quadColor = { 0.2f, 0.3f, 0.8f, 1.0f };
		float m_quadRotation = 0.0f;

#if defined MN_ENABLE_DEBUG_CODE
		Renderer2D::Statistics m_renderer2DStatistics;
#endif
	};

}