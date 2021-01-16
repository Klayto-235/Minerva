#pragma once

#include <Minerva/core/Layer.h>


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
		OrthographicCameraController m_cameraController;

		Ref<Texture2D> m_chessboardTexture;

		Ref<Framebuffer> m_framebuffer;

		glm::vec4 m_quadColor = { 0.2f, 0.3f, 0.8f, 1.0f };
		float m_quadRotation = 0.0f;

		Renderer2D::Statistics m_renderer2DStatistics;
	};

}