#pragma once

#include "Minerva/Renderer/Camera.h"


namespace Minerva
{

	class SceneCamera : public Camera
	{
	public:
		enum class ProjectionType { Perspective = 0, Orthographic = 1 };

		SceneCamera() { calculateProjection(); }
		virtual ~SceneCamera() = default;

		ProjectionType getProjectionType() const { return m_projectionType; }
		void  setProjectionType(ProjectionType type) { m_projectionType = type; calculateProjection(); }
		float getAspectRatio() const { return m_aspectRatio; }
		void  setAspectRatio(float aspectRatio) { m_aspectRatio = aspectRatio; calculateProjection(); }

		void  setPerspective(float yFOV, float zNear, float zFar, float aspectRatio = 0.0f);
		float getPerspectiveVerticalFOV() const { return m_perspectiveYFOV; }
		void  setPerspectiveVerticalFOV(float yFOV) { m_perspectiveYFOV = yFOV; calculateProjection(); }
		float getPerspectiveNearClip() const { return m_perspectiveZNear; }
		void  setPerspectiveNearClip(float zNear) { m_perspectiveZNear = zNear; calculateProjection(); }
		float getPerspectiveFarClip() const { return m_perspectiveZFar; }
		void  setPerspectiveFarClip(float zFar) { m_perspectiveZFar = zFar; calculateProjection(); }

		void  setOrthographic(float yHalfSpan, float zNear, float zFar, float aspectRatio = 0.0f);
		float getOrthographicSize() const { return m_orthographicYHalfSpan; }
		void  setOrthographicSize(float yHalfSpan) { m_orthographicYHalfSpan = yHalfSpan; calculateProjection(); }
		float getOrthographicNearClip() const { return m_orthographicZNear; }
		void  setOrthographicNearClip(float zNear) { m_orthographicZNear = zNear; calculateProjection(); }
		float getOrthographicFarClip() const { return m_orthographicZFar; }
		void  setOrthographicFarClip(float zFar) { m_orthographicZFar = zFar; calculateProjection(); }
	private:
		void calculateProjection();

		ProjectionType m_projectionType = ProjectionType::Perspective;
		float m_aspectRatio = 1.0f;
		float m_perspectiveYFOV = glm::radians(45.0f);
		float m_perspectiveZNear = 0.01f, m_perspectiveZFar = 1000.0f;
		float m_orthographicYHalfSpan = 1.0f;
		float m_orthographicZNear = -1.0f, m_orthographicZFar = 1.0f;
	};

}