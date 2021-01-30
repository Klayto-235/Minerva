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

	Entity Scene::newEntity(const std::string& name)
	{
		Entity entity = { m_registry.create(), this };
		entity.addComponent<TagComponent>(name);
		return entity;
	}

	void Scene::onUpdate(const float timeStep, const InputState& inputState)
	{

	}

	void Scene::onRender(Renderer2D& renderer2D)
	{
		if (m_mainCamera)
		{
			{
				// entt::registry::get doesn't support const template parameters so we are using
				// const_cast as a workaround.
				auto& [cameraComponent, cameraTransformComponent] =
					const_cast<Entity*>(m_mainCamera)->getComponents<CameraComponent, TransformComponent>();
				renderer2D.beginScene(cameraComponent.camera, cameraTransformComponent.matrix);
			}

			auto group = m_registry.group<Transform2DComponent>(entt::get<SpriteRenderComponent>);
			for (auto entity : group)
			{
				auto& [transformComponent, spriteRenderComponent] =
					group.get<Transform2DComponent, SpriteRenderComponent>(entity);

				renderer2D.drawQuad(transformComponent.matrix, spriteRenderComponent.color, transformComponent.z);
			}

			renderer2D.endScene();
		}
	}

	void Scene::onViewportResize(uint32_t width, uint32_t height)
	{
		const float aspectRatio = (float)width / (float)height;

		auto view = m_registry.view<CameraComponent>();
		for (auto entity : view)
		{
			auto& cameraComponent = view.get<CameraComponent>(entity);
			if (!cameraComponent.fixedAspectRatio)
				cameraComponent.camera.setAspectRatio(aspectRatio);
		}
	}

}