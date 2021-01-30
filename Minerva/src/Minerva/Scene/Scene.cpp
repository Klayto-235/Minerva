#include "mnpch.h"
#include "Minerva/Scene/Scene.h"
#include "Minerva/Scene/Entity.h"
#include "Minerva/Scene/Components.h"


namespace Minerva
{

	Scene::Scene()
	{
	}

	Scene::~Scene()
	{
	}

	Entity Scene::newEntity()
	{
		return { m_registry.create(), this };
	}

	void Scene::onUpdate(const float timeStep, const InputState& inputState)
	{
	}

	void Scene::onRender(Renderer2D& renderer2D)
	{
		auto group = m_registry.group<Transform2DComponent>(entt::get<SpriteRenderComponent>);
		for (auto entity : group)
		{
			auto& [transform, sprite] = group.get<Transform2DComponent, SpriteRenderComponent>(entity);

			renderer2D.drawQuad(transform.matrix, sprite.color, transform.z);
		}
	}

}