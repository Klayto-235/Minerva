#pragma once

#include "Minerva/core/core.h"
#include "Minerva/core/InputState.h"
#include "Minerva/Renderer/Renderer2D.h"
#include "Minerva/Scene/Components.h"

#include <string>

#include <entt.hpp>


namespace Minerva
{
	class Scene;
	class NativeScriptBase;

	class Entity
	{
	public:
		Entity() = default;
		Entity(entt::entity handle, Scene* scene)
			: m_handle(handle), m_scene(scene) {}

		template<typename T, typename ...Args>
		T& addComponent(Args&&... args);

		template<typename T>
		NativeScriptComponent& addNativeScript();

		template<typename ...T>
		decltype(auto) getComponents();

		template<typename ...T>
		auto tryGetComponents();

		template<typename ...T>
		bool hasComponents() const;

		template<typename ...T>
		void removeComponents();

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

	class Scene
	{
	public:
		Scene(uint32_t viewportWidth = 1280, uint32_t viewportheight = 720)
			: m_aspectRatio((float)viewportWidth / (float)viewportheight) {}

		Entity newEntity();
		Entity newEntity(const std::string& name);
		void deleteEntity(Entity entity);

		void setViewportSize(uint32_t width, uint32_t height);
		void setMainCamera(Entity camera) { m_mainCameraEntity = camera; }

		void onStart();
		void onStop();
		void onUpdate(const float timeStep, const InputState& inputState);
		void onRender(Renderer2D& renderer2D);
	private:
		friend class Entity;
		friend class SceneHierarchyPanel;
		friend class PropertiesPanel;

		template<typename T>
		void onAddComponent(Entity entity, T& component);

		entt::registry m_registry;

		bool m_running = false;
		float m_aspectRatio;
		Entity m_mainCameraEntity;
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

	template<typename T>
	inline void Scene::onAddComponent(Entity entity, T& component)
	{
		static_assert("Invalid template argument: calling primary template instead of specialisation."); // TODO: Why is this not causing a compilation error? We don't have specialisations for all components.
	}

	template<>
	inline void Scene::onAddComponent(Entity entity, NativeScriptComponent& component)
	{
		if (m_running)
		{
			component.script.reset(component.instantiateScript());
			component.script->m_entity = { entity, this };
			component.script->onCreate();
		}
	}

	template<>
	inline void Scene::onAddComponent(Entity entity, CameraComponent& component)
	{
		if (!component.fixedAspectRatio)
			component.camera.setAspectRatio(m_aspectRatio);
	}

	template<typename T, typename ...Args>
	inline T& Entity::addComponent(Args&&... args)
	{
		static_assert(std::negation<std::is_same<NativeScriptComponent, T>>::value,
			"Invalid component type. Use addNativeScript instead.");
		MN_CORE_ASSERT(!hasComponents<T>(), "Entity already has the specified component.");
		auto& component = m_scene->m_registry.emplace<T>(m_handle, std::forward<Args>(args)...);
		m_scene->onAddComponent<T>(*this, component);
		return component;
	}

	template<typename T>
	inline NativeScriptComponent& Entity::addNativeScript()
	{
		MN_CORE_ASSERT(!hasComponents<NativeScriptComponent>(), "Entity already has a native script component.");
		auto& scriptComponent = m_scene->m_registry.emplace<NativeScriptComponent>(m_handle);
		scriptComponent.bind<T>();
		m_scene->onAddComponent<NativeScriptComponent>(*this, scriptComponent);
		return scriptComponent;
	}

	template<typename ...T>
	inline decltype(auto) Entity::getComponents()
	{
		MN_CORE_ASSERT(hasComponents<T...>(), "Entity does not have the specified components.");
		return m_scene->m_registry.get<T...>(m_handle);
	}

	template<typename ...T>
	inline auto Entity::tryGetComponents()
	{
		return m_scene->m_registry.try_get<T...>(m_handle);
	}

	template<typename ...T>
	inline bool Entity::hasComponents() const
	{
		return m_scene->m_registry.has<T...>(m_handle);
	}

	template<typename ...T>
	inline void Entity::removeComponents()
	{
		MN_CORE_ASSERT(hasComponents<T...>(), "Entity does not have the specified components.");
		if constexpr (std::disjunction_v<std::is_same<NativeScriptComponent, T>...>)
		{
			if (m_scene->m_running)
			{
				auto& scriptComponent = m_scene->m_registry.get<NativeScriptComponent>(m_handle);
				scriptComponent.script->onDestroy();
			}
		}
		m_scene->m_registry.remove<T...>(m_handle);
	}

}