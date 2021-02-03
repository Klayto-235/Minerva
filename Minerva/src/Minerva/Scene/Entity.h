#pragma once

#include "Minerva/core/core.h"
#include "Minerva/Scene/Scene.h"

#include <entt.hpp>


namespace Minerva
{
	struct NativeScriptComponent;

	class Entity
	{
	public:
		Entity() = default;
		Entity(entt::entity handle, Scene* scene)
			: m_handle(handle), m_scene(scene) {}

		template<typename T, typename ...Args>
		T& addComponent(Args&&... args)
		{
			MN_CORE_ASSERT(!hasComponents<T>(), "Entity already has the specified component.");
			return m_scene->m_registry.emplace<T>(m_handle, std::forward<Args>(args)...);
		}

		template<typename T>
		NativeScriptComponent& addNativeScript()
		{
			auto& scriptComponent = addComponent<NativeScriptComponent>();
			scriptComponent.bind<T>();
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
			m_scene->m_registry.remove<T...>(m_handle);
		}

		operator uint32_t() const { return static_cast<uint32_t>(m_handle); }

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