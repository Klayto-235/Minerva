#pragma once

#include "Minerva/Scene/SceneCamera.h"

#include <string>

#include <glm/glm.hpp>


namespace Minerva
{

	struct TagComponent
	{
		std::string name;

		TagComponent() = default;
		TagComponent(const std::string& name)
			: name(name) {}
	};

	struct TransformComponent
	{
		glm::mat4 matrix{ 1.0f };

		TransformComponent() = default;
		TransformComponent(const glm::mat4& transform)
			: matrix(transform) {}
	};

	struct Transform2DComponent
	{
		glm::mat3 matrix{ 1.0f };
		float z = 0.0f;

		Transform2DComponent() = default;
		Transform2DComponent(const glm::mat3& transform, float z = 0.0f)
			: matrix(transform), z(z) {}
	};

	struct SpriteRenderComponent
	{
		glm::vec4 color{ 1.0f, 1.0f, 1.0f, 1.0f };

		SpriteRenderComponent() = default;
		SpriteRenderComponent(const glm::vec4& color)
			: color(color) {}
	};

	struct CameraComponent
	{
		SceneCamera camera;
		bool fixedAspectRatio = false;
	};

}