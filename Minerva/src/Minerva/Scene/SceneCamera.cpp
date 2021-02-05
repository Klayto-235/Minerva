#include "mnpch.h"
#include "SceneCamera.h"

#include <glm/gtc/matrix_transform.hpp>


namespace Minerva
{

	void SceneCamera::setPerspective(float yFOV, float zNear, float zFar, float aspectRatio)
	{
		m_projectionType = ProjectionType::Perspective;
		m_perspectiveYFOV = yFOV;
		m_perspectiveZNear = zNear;
		m_perspectiveZFar = zFar;
		if (aspectRatio != 0.0f)
			m_aspectRatio = aspectRatio;
		calculateProjection();
	}

	void SceneCamera::setOrthographic(float yHalfSpan, float zNear, float zFar, float aspectRatio)
	{
		m_projectionType = ProjectionType::Orthographic;
		m_orthographicYHalfSpan = yHalfSpan;
		m_orthographicZNear = zNear;
		m_orthographicZFar = zFar;
		if (aspectRatio != 0.0f)
			m_aspectRatio = aspectRatio;
		calculateProjection();
	}

	void SceneCamera::calculateProjection()
	{
		if (m_projectionType == ProjectionType::Perspective)
			m_projection = glm::perspective(
				m_perspectiveYFOV,
				m_aspectRatio,
				m_perspectiveZNear,
				m_perspectiveZFar);
		else
			m_projection = glm::ortho(
				-m_orthographicYHalfSpan * m_aspectRatio,
				m_orthographicYHalfSpan * m_aspectRatio,
				-m_orthographicYHalfSpan,
				m_orthographicYHalfSpan,
				m_orthographicZNear,
				m_orthographicZFar);
	}

}