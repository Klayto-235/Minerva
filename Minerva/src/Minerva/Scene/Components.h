#pragma once

#include "Minerva/core/core.h"
#include "Minerva/Scene/SceneCamera.h"

#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


namespace Minerva
{
	class NativeScriptBase;

	struct TagComponent
	{
		std::string name;

		TagComponent() = default;
		explicit TagComponent(const std::string& name)
			: name(name) {}
	};

	struct TransformComponent
	{
		glm::vec3 translation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 rotation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 scale = { 1.0f, 1.0f, 1.0f };

		glm::mat4 getTransform(bool inverse = false) const
		{
			if (inverse)
			{
				glm::mat4 rotationMatrix =
					glm::rotate(glm::mat4(1.0f), -rotation.x, { 1, 0, 0 }) *
					glm::rotate(glm::mat4(1.0f), -rotation.y, { 0, 1, 0 }) *
					glm::rotate(glm::mat4(1.0f), -rotation.z, { 0, 0, 1 });

				return glm::scale(glm::translate(glm::mat4(1.0f), -translation) * rotationMatrix, glm::vec3(1.0f)/scale);
			}
			else
			{
				glm::mat4 rotationMatrix =
					glm::rotate(glm::mat4(1.0f), rotation.x, { 1, 0, 0 }) *
					glm::rotate(glm::mat4(1.0f), rotation.y, { 0, 1, 0 }) *
					glm::rotate(glm::mat4(1.0f), rotation.z, { 0, 0, 1 });

				return glm::scale(glm::translate(glm::mat4(1.0f), translation) * rotationMatrix, scale);
			}
		}
	};

	struct SpriteRenderComponent
	{
		glm::vec4 color{ 1.0f, 1.0f, 1.0f, 1.0f };

		SpriteRenderComponent() = default;
		explicit SpriteRenderComponent(const glm::vec4& color)
			: color(color) {}
	};

	struct CameraComponent
	{
		SceneCamera camera;
		bool fixedAspectRatio = false;
	};

	struct NativeScriptComponent
	{
		Scope<NativeScriptBase> script;

		NativeScriptBase* (*instantiateScript)();

		template<typename T>
		void bind()
		{
			instantiateScript = []() { return static_cast<NativeScriptBase*>(new T()); };
		}
	};

}