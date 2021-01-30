#pragma once

#include <glm/glm.hpp>


namespace Minerva
{

	class Camera
	{
	public:
		Camera() = default;
		Camera(glm::mat4 projection)
			: m_projection(projection) {}
		virtual ~Camera() = default;

		const glm::mat4& getProjection() const { return m_projection; }
	protected:
		glm::mat4 m_projection = glm::mat4(1.0f);
	};

	class OrthographicCamera
	{
	public:
		OrthographicCamera(float width, float height, float depth = 2.0f);
		OrthographicCamera(float width, float height, float zNear, float zFar);

		void setProjection(float width, float height, float depth = 2.0f);
		void setProjection(float width, float height, float zNear, float zFar);
		
		void setPosition(const glm::vec3& position) { m_position = position; calculateViewMatrix(); }
		void setRotation(float rotation) { m_rotation = rotation; calculateViewMatrix(); }
		const glm::vec3& getPosition() const { return m_position; }
		float getRotation() const { return m_rotation; }

		const glm::mat4 getProjectionMatrix() const { return m_projectionMatrix; }
		const glm::mat4 getViewMatrix() const { return m_viewMatrix; }
		const glm::mat4 getViewProjectionMatrix() const { return m_viewProjectionMatrix; }
	private:
		glm::vec3 m_position;
		float m_rotation;
		glm::mat4 m_projectionMatrix;
		glm::mat4 m_viewMatrix;
		glm::mat4 m_viewProjectionMatrix;

		void calculateViewMatrix();
	};

}