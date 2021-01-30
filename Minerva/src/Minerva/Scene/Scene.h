#pragma once

#include "Minerva/core/InputState.h"
#include "Minerva/Renderer/Renderer2D.h"

#include <string>

#include <entt.hpp>


namespace Minerva
{
	class Entity;

	class Scene
	{
	public:
		Scene();
		~Scene();

		Entity newEntity();
		Entity newEntity(const std::string& name);

		void setMainCamera(const Entity* camera) { m_mainCamera = camera; }

		void onUpdate(const float timeStep, const InputState& inputState);
		void onRender(Renderer2D& renderer2D);

		void onViewportResize(uint32_t width, uint32_t height);
	private:
		friend class Entity;

		entt::registry m_registry;

		const Entity* m_mainCamera = nullptr;
	};

}