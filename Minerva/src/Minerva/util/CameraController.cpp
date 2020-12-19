#include "mnpch.h"
#include "Minerva/util/CameraController.h"
#include "Minerva/Events/MouseEvent.h"
#include "Minerva/Events/WindowEvent.h"


namespace Minerva
{

	void OrthographicCameraController::onUpdate(float timeStep, const Window::InputState& inputState)
	{
		if (inputState.isKeyPressed(Minerva::Key::A))
		{
			m_cameraPosition.x -= cos(glm::radians(m_cameraRotation)) * m_cameraTranslationSpeed * timeStep;
			m_cameraPosition.y -= sin(glm::radians(m_cameraRotation)) * m_cameraTranslationSpeed * timeStep;
		}
		else if (inputState.isKeyPressed(Minerva::Key::D))
		{
			m_cameraPosition.x += cos(glm::radians(m_cameraRotation)) * m_cameraTranslationSpeed * timeStep;
			m_cameraPosition.y += sin(glm::radians(m_cameraRotation)) * m_cameraTranslationSpeed * timeStep;
		}

		if (inputState.isKeyPressed(Minerva::Key::W))
		{
			m_cameraPosition.x += -sin(glm::radians(m_cameraRotation)) * m_cameraTranslationSpeed * timeStep;
			m_cameraPosition.y += cos(glm::radians(m_cameraRotation)) * m_cameraTranslationSpeed * timeStep;
		}
		else if (inputState.isKeyPressed(Minerva::Key::S))
		{
			m_cameraPosition.x -= -sin(glm::radians(m_cameraRotation)) * m_cameraTranslationSpeed * timeStep;
			m_cameraPosition.y -= cos(glm::radians(m_cameraRotation)) * m_cameraTranslationSpeed * timeStep;
		}

		if (m_rotation) {
			if (inputState.isKeyPressed(Minerva::Key::Q)) m_cameraRotation += m_cameraRotationSpeed * timeStep;
			if (inputState.isKeyPressed(Minerva::Key::E)) m_cameraRotation -= m_cameraRotationSpeed * timeStep;

			if (m_cameraRotation > 180.0f) m_cameraRotation -= 360.0f;
			else if (m_cameraRotation <= -180.0f) m_cameraRotation += 360.0f;

			m_camera.setRotation(m_cameraRotation);
		}

		m_camera.setPosition(m_cameraPosition);
	}

	bool OrthographicCameraController::onEvent(const Event& event)
	{
		const EventType eventType = event.getEventType();
		if (eventType == EventType::MouseScroll)
		{
			m_zoom -= static_cast<const MouseScrollEvent&>(event).getYOffset() * 0.25f;
			m_zoom = std::max(m_zoom, 0.25f);
			m_cameraTranslationSpeed = m_zoom;
			m_camera.setProjection(2.0f*m_zoom*m_aspectRatio, 2.0f*m_zoom);
			return true;
		}
		else if (eventType == EventType::WindowResize)
		{
			const WindowResizeEvent& e = static_cast<const WindowResizeEvent&>(event);
			m_aspectRatio = (float)e.getWidth() / (float)e.getHeight();
			m_camera.setProjection(2.0f*m_zoom*m_aspectRatio, 2.0f*m_zoom);
			return false;
		}
		else return false;
	}

}