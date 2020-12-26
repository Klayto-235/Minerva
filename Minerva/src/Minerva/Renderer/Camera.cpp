#include "mnpch.h"
#include "Minerva/Renderer/Camera.h"

#include <glm/gtc/matrix_transform.hpp>


namespace Minerva
{

	OrthographicCamera::OrthographicCamera(float width, float height, float depth)
		: m_position(0.0f, 0.0f, 0.0f), m_rotation(0.0f), m_viewMatrix(1.0f),
		m_projectionMatrix(glm::ortho(-width/2.0f, width/2.0f, -height/2.0f, height/2.0f, -depth/2.0f, depth/2.0f))
	{
		MN_PROFILE_FUNCTION();

		m_viewProjectionMatrix = m_projectionMatrix * m_viewMatrix;
	}

	OrthographicCamera::OrthographicCamera(float width, float height, float zNear, float zFar)
		: m_position(0.0f, 0.0f, 0.0f), m_rotation(0.0f), m_viewMatrix(1.0f),
		m_projectionMatrix(glm::ortho(-width/2.0f, width/2.0f, -height/2.0f, height/2.0f, zNear, zFar))
	{
		MN_PROFILE_FUNCTION();

		m_viewProjectionMatrix = m_projectionMatrix * m_viewMatrix;
	}

	void OrthographicCamera::setProjection(float width, float height, float depth)
	{
		MN_PROFILE_FUNCTION();

		m_projectionMatrix = glm::ortho(-width/2.0f, width/2.0f, -height/2.0f, height/2.0f, -depth/2.0f, depth/2.0f);
		m_viewProjectionMatrix = m_projectionMatrix * m_viewMatrix;
	}

	void OrthographicCamera::setProjection(float width, float height, float zNear, float zFar)
	{
		MN_PROFILE_FUNCTION();

		m_projectionMatrix = glm::ortho(-width/2.0f, width/2.0f, -height/2.0f, height/2.0f, zNear, zFar);
		m_viewProjectionMatrix = m_projectionMatrix * m_viewMatrix;
	}

	void OrthographicCamera::calculateViewMatrix()
	{
		MN_PROFILE_FUNCTION();

		m_viewMatrix = glm::translate(glm::rotate(glm::mat4(1.0f),
			-glm::radians(m_rotation), glm::vec3(0, 0, 1)), -m_position);

		m_viewProjectionMatrix = m_projectionMatrix * m_viewMatrix;
	}

}