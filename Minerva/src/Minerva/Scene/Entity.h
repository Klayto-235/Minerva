#pragma once

#include "Scene.h"

#include <entt.hpp>


namespace Minerva
{

	class Entity
	{
	public:
		Entity() = default;
		Entity(entt::entity handle, Scene* scene)
			: m_handle(handle), m_scene(scene) {}

		template<typename T, typename ...Args>
		T& addComponent(Args&&... args)
		{
			MN_CORE_ASSERT(!hasComponent<T>(), "Entity already has component.");
			return m_scene->m_registry.emplace<T>(m_handle, std::forward<Args>(args)...);
		}

		template<typename T>
		T& getComponent()
		{
			MN_CORE_ASSERT(hasComponent<T>(), "Entity does not have component.");
			return m_scene->m_registry.get<T>(m_handle);
		}

		template<typename T>
		bool hasComponent()
		{
			return m_scene->m_registry.has<T>(m_handle);
		}

		template<typename T>
		void removeComponent()
		{
			MN_CORE_ASSERT(hasComponent<T>(), "Entity does not have component.");
			m_scene->m_registry.remove<T>(m_handle);
		}

		operator bool() const { return m_handle != entt::null; }
	private:
		entt::entity m_handle{ entt::null };
		Scene* m_scene = nullptr;
	};

}