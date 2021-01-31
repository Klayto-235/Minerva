#pragma once

#include "Minerva/core/core.h"
#include "Minerva/Scene/SceneCamera.h"
#include "Minerva/Scene/Entity.h"
#include "Minerva/core/InputState.h"

#include <string>

#include <glm/glm.hpp>


namespace Minerva
{

	struct TagComponent
	{
		std::string name;

		TagComponent() = default;
		explicit TagComponent(const std::string& name)
			: name(name) {}
	};

	struct TransformComponent
	{
		glm::mat4 matrix{ 1.0f };

		TransformComponent() = default;
		explicit TransformComponent(const glm::mat4& transform)
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
		explicit SpriteRenderComponent(const glm::vec4& color)
			: color(color) {}
	};

	struct CameraComponent
	{
		SceneCamera camera;
		bool fixedAspectRatio = false;
	};

	class NativeScriptBase
	{
	public:
		virtual ~NativeScriptBase() = default;

		virtual void onCreate() {}
		virtual void onDestroy() {}
		virtual void onUpdate(float timeStep, const InputState& inputState) {}

		template<typename ...T>
		decltype(auto) getComponents()
		{
			MN_CORE_ASSERT(m_entity, "Native script used with invalid Entity.")
			return m_entity.getComponents<T...>();
		}
	protected:
		NativeScriptBase() = default;
	private:
		friend class Scene;

		Entity m_entity;
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