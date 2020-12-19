#pragma once

#include "Minerva/Renderer/Camera.h"
#include "Minerva/Events/Event.h"
#include "Minerva/core/Window.h"

#include <glm/glm.hpp>


namespace Minerva
{

	class OrthographicCameraController
	{
	public:
		OrthographicCameraController(float aspectRatio, bool rotation = false)
			: m_camera(2.0f*aspectRatio*m_zoom, 2.0f*m_zoom), m_rotation(rotation),
			m_aspectRatio(aspectRatio) {}

		void onUpdate(float timeStep, const Window::InputState& inputState);
		bool onEvent(const Event& event);

		OrthographicCamera& getCamera() { return m_camera; }
	private:
		bool m_rotation;
		float m_aspectRatio;
		float m_zoom = 1.0f;
		OrthographicCamera m_camera;

		glm::vec3 m_cameraPosition = { 0.0f, 0.0f, 0.0f };
		float m_cameraTranslationSpeed = 5.0f;
		float m_cameraRotation = 0.0f;
		float m_cameraRotationSpeed = 180.0f;
	};

}