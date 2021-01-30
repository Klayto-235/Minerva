#include "mnpch.h"
#include "SceneCamera.h"

#include <glm/gtc/matrix_transform.hpp>


namespace Minerva
{

	SceneCamera::SceneCamera()
	{
		calculateProjection();
	}

	SceneCamera::SceneCamera(float yHalfSpan, float zNear, float zFar, float aspectRatio)
		: m_orthographicYHalfSpan(yHalfSpan), m_aspectRatio(aspectRatio),
		m_orthographicZNear(zNear), m_orthographicZFar(zFar)
	{
		calculateProjection();
	}

	void SceneCamera::setOrthographicProjection(float yHalfSpan, float zNear, float zFar, float aspectRatio)
	{
		m_orthographicYHalfSpan = yHalfSpan;
		m_orthographicZNear = zNear;
		m_orthographicZFar = zFar;
		if (aspectRatio != 0.0f)
			m_aspectRatio = aspectRatio;
		calculateProjection();
	}

	void SceneCamera::calculateProjection()
	{
		m_projection = glm::ortho(
			-m_orthographicYHalfSpan * m_aspectRatio,
			m_orthographicYHalfSpan * m_aspectRatio,
			-m_orthographicYHalfSpan,
			m_orthographicYHalfSpan,
			m_orthographicZNear,
			m_orthographicZFar);
	}

}