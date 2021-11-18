#pragma once

#include "Minerva/core/InputState.h"
#include "Minerva/Renderer/Renderer2D.h"
#include "Minerva/Scene/Entity.h"
#include "Minerva/Scene/Components.h"

#include <string>

#include <entt.hpp>


namespace Minerva
{

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

}