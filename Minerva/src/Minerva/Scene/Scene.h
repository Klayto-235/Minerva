#pragma once

#include "Minerva/core/InputState.h"
#include "Minerva/Renderer/Renderer2D.h"

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

		void onUpdate(const float timeStep, const InputState& inputState);
		void onRender(Renderer2D& renderer2D);
	private:
		friend class Entity;

		entt::registry m_registry;
	};

}