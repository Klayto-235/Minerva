#include "mnpch.h"
#include "Minerva/Scene/Scene.h"
#include "Minerva/Scene/Entity.h"
#include "Minerva/Scene/Components.h"


namespace Minerva
{

	Entity Scene::newEntity()
	{
		return { m_registry.create(), &m_registry };
	}

	Entity Scene::newEntity(const std::string& name)
	{
		Entity entity = { m_registry.create(), &m_registry };
		entity.addComponent<TagComponent>(name);
		return entity;
	}

	void Scene::onStart()
	{
		auto view = m_registry.view<NativeScriptComponent>();
		for (auto entity : view)
		{
			auto& scriptComponent = view.get<NativeScriptComponent>(entity);
			scriptComponent.script.reset(scriptComponent.instantiateScript());
			scriptComponent.script->m_entity = { entity, &m_registry };
			scriptComponent.script->onCreate();
		}
	}

	void Scene::onStop()
	{
		auto view = m_registry.view<NativeScriptComponent>();
		for (auto entity : view)
		{
			auto& scriptComponent = view.get<NativeScriptComponent>(entity);
			scriptComponent.script->onDestroy();
			scriptComponent.script.reset();
		}
	}

	void Scene::onUpdate(const float timeStep, const InputState& inputState)
	{
		auto view = m_registry.view<NativeScriptComponent>();
		for (auto entity : view)
			view.get<NativeScriptComponent>(entity).script->onUpdate(timeStep, inputState);
	}

	void Scene::onRender(Renderer2D& renderer2D)
	{
		if (m_mainCameraEntity)
		{
			auto [cameraComponent, cameraTransformComponent] =
				m_mainCameraEntity.getComponents<CameraComponent, TransformComponent>();
			renderer2D.beginScene(cameraComponent.camera, cameraTransformComponent.getTransform(true));

			auto group = m_registry.group<TransformComponent>(entt::get<SpriteRenderComponent>);
			for (auto entity : group)
			{
				auto [transformComponent, spriteRenderComponent] =
					group.get<TransformComponent, SpriteRenderComponent>(entity);

				renderer2D.drawQuad(transformComponent.getTransform(), spriteRenderComponent.color);
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