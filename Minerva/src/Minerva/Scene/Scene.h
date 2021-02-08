#pragma once

#include "Minerva/core/InputState.h"
#include "Minerva/Renderer/Renderer2D.h"
#include "Minerva/Scene/Entity.h"

#include <string>

#include <entt.hpp>


namespace Minerva
{

	class Scene
	{
	public:
		Entity newEntity();
		Entity newEntity(const std::string& name);

		void setMainCamera(Entity camera) { m_mainCameraEntity = camera; }

		void onStart();
		void onStop();
		void onUpdate(const float timeStep, const InputState& inputState);
		void onRender(Renderer2D& renderer2D);

		void onViewportResize(uint32_t width, uint32_t height);
	private:
		friend class SceneHierarchyPanel;
		friend class PropertiesPanel;

		entt::registry m_registry;

		Entity m_mainCameraEntity;
	};

}