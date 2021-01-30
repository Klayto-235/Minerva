#pragma once

#include "Minerva/Renderer/Camera.h"


namespace Minerva
{

	class SceneCamera : public Camera
	{
	public:
		SceneCamera();
		SceneCamera(float yHalfSpan, float zNear, float zFar, float aspectRatio = 1.0f);
		virtual ~SceneCamera() = default;

		void setOrthographicProjection(float yHalfSpan, float zNear, float zFar, float aspectRatio = 0.0f);
		void setOrthographicSize(float yHalfSpan) { m_orthographicYHalfSpan = yHalfSpan; calculateProjection(); }
		void setAspectRatio(float aspectRatio) { m_aspectRatio = aspectRatio; calculateProjection(); }
	private:
		void calculateProjection();

		float m_orthographicYHalfSpan = 1.0f;
		float m_orthographicZNear = -1.0f, m_orthographicZFar = 1.0f;
		float m_aspectRatio = 1.0f;
	};

}