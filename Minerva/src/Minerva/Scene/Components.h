#pragma once

#include "Minerva/core/core.h"
#include "Minerva/Scene/SceneCamera.h"
#include "Minerva/Scene/Entity.h"
#include "Minerva/core/InputState.h"

#include <string>
#include <type_traits>

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
		template<typename ...T>
		decltype(auto) getComponents()
		{
			MN_CORE_ASSERT(m_entity, "Native script used with invalid Entity.")
			return m_entity.getComponents<T...>();
		}
	protected:
		NativeScriptBase() = default;
	private:
		friend class NativeScriptComponent;

		Entity m_entity;
	};

	class NativeScriptComponent
	{
	public:
		template<typename T>
		NativeScriptComponent(Scope<T>&& script, const Entity& entity)
			: m_script(std::move(script))
		{
			static_assert(std::is_base_of<NativeScriptBase, T>::value,
				"Invalid template argument: native script class must inherit from NativeScriptBase.");

			m_script->m_entity = entity;

			impl_onCreate = [](NativeScriptBase* script) { static_cast<T*>(script)->onCreate(); };
			impl_onDestroy = [](NativeScriptBase* script) { static_cast<T*>(script)->onDestroy(); };
			impl_onUpdate = [](NativeScriptBase* script, float timeStep, const InputState& inputState)
				{ static_cast<T*>(script)->onUpdate(timeStep, inputState); };
			
			impl_onCreate(m_script.get());
		}

		NativeScriptComponent(const NativeScriptComponent& other) = delete;
		NativeScriptComponent& operator=(const NativeScriptComponent& other) = delete;

		NativeScriptComponent(NativeScriptComponent&& other) noexcept
			: m_script(std::move(other.m_script)), impl_onCreate(std::move(other.impl_onCreate)),
			impl_onDestroy(std::move(other.impl_onDestroy)), impl_onUpdate(std::move(other.impl_onUpdate)) {}
		NativeScriptComponent& operator=(NativeScriptComponent&& other) noexcept
		{
			m_script = std::move(other.m_script);
			impl_onCreate = std::move(other.impl_onCreate);
			impl_onDestroy = std::move(other.impl_onDestroy);
			impl_onUpdate = std::move(other.impl_onUpdate);
			return *this;
		}

		~NativeScriptComponent() { impl_onDestroy(m_script.get()); }

		void onCreate() { impl_onCreate(m_script.get()); }
		void onDestroy() { impl_onDestroy(m_script.get()); }
		void onUpdate(float timeStep, const InputState& inputState)
			{ impl_onUpdate(m_script.get(), timeStep, inputState); }
	private:
		Scope<NativeScriptBase> m_script;

		std::function<void(NativeScriptBase*)> impl_onCreate;
		std::function<void(NativeScriptBase*)> impl_onDestroy;
		std::function<void(NativeScriptBase*, float, const InputState&)> impl_onUpdate;
	};

}