#pragma once

#include <glm/glm.hpp>
#include <string>


namespace Minerva
{

	struct TagComponent
	{
		std::string tag;

		TagComponent() = default;
		TagComponent(const std::string& tag)
			: tag(tag) {}
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

}