#pragma once

#include "Minerva/core/core.h"

#include <entt.hpp>


namespace Minerva
{
	struct NativeScriptComponent;
	class Scene;

	class Entity
	{
	public:
		Entity() = default;
		Entity(entt::entity handle, Scene* scene)
			: m_handle(handle), m_scene(scene) {}

		template<typename T, typename ...Args>
		T& addComponent(Args&&... args)
		{
			static_assert(std::negation<std::is_same<NativeScriptComponent, T>>::value,
				"Invalid component type. Use addNativeScript instead.");
			MN_CORE_ASSERT(!hasComponents<T>(), "Entity already has the specified component.");
			auto& component = m_scene->m_registry.emplace<T>(m_handle, std::forward<Args>(args)...);
			m_scene->onAddComponent<T>(*this, component);
			return component;
		}

		template<typename T>
		NativeScriptComponent& addNativeScript()
		{
			MN_CORE_ASSERT(!hasComponents<NativeScriptComponent>(), "Entity already has a native script component.");
			auto& scriptComponent = m_scene->m_registry.emplace<NativeScriptComponent>(m_handle);
			scriptComponent.bind<T>();
			m_scene->onAddComponent<NativeScriptComponent>(*this, scriptComponent);
			return scriptComponent;
		}

		template<typename ...T>
		decltype(auto) getComponents()
		{
			MN_CORE_ASSERT(hasComponents<T...>(), "Entity does not have the specified components.");
			return m_scene->m_registry.get<T...>(m_handle);
		}

		template<typename ...T>
		auto tryGetComponents()
		{
			return m_scene->m_registry.try_get<T...>(m_handle);
		}

		template<typename ...T>
		bool hasComponents() const
		{
			return m_scene->m_registry.has<T...>(m_handle);
		}

		template<typename ...T>
		void removeComponents()
		{
			MN_CORE_ASSERT(hasComponents<T...>(), "Entity does not have the specified components.");
			if constexpr (std::disjunction_v<std::is_same<NativeScriptComponent, T>...>)
			{
				if (m_scene->m_running)
				{
					auto& scriptComponent = view.get<NativeScriptComponent>(entity);
					scriptComponent.script->onDestroy();
				}
			}
			m_scene->m_registry.remove<T...>(m_handle);
		}

		operator uint32_t() const { return static_cast<uint32_t>(m_handle); }
		operator entt::entity() const { return m_handle; }

		operator bool() const { return m_handle != entt::null; }

		bool operator==(const Entity& other) const
			{ return m_handle == other.m_handle && m_scene == other.m_scene; }

		bool operator!=(const Entity& other) const
			{ return m_handle != other.m_handle || m_scene != other.m_scene; }
	private:
		entt::entity m_handle{ entt::null };
		Scene* m_scene = nullptr;
	};

}