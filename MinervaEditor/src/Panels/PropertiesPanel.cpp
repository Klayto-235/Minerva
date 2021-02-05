#include "Panels/PropertiesPanel.h"
#include "EditorLayer.h"

#include <imgui.h>

#include <glm/gtc/type_ptr.hpp>


namespace Minerva
{

	void PropertiesPanel::onImGuiRender()
	{
		if (ImGui::Begin("Properties"))
		{
			switch (m_state->selection.type)
			{
			case EditorContext::Type::Entity: drawEntityProperties(*static_cast<Entity*>(m_state->selection.ptr)); break;
			case EditorContext::Type::Scene: drawSceneProperties(static_cast<Scene*>(m_state->selection.ptr)); break;
			}
		}
		ImGui::End();
	}

	void PropertiesPanel::drawSceneProperties(Scene* scene)
	{
		auto tagComponent = scene->m_mainCameraEntity.tryGetComponents<TagComponent>();
		if (ImGui::BeginCombo("Main Camera", tagComponent ? tagComponent->name.c_str() : "Unnamed Entity"))
		{
			auto view = scene->m_registry.view<CameraComponent>();
			for (auto entityHandle : view)
			{
				Entity entity{ entityHandle, &scene->m_registry };
				tagComponent = entity.tryGetComponents<TagComponent>();

				bool selected = (entity == scene->m_mainCameraEntity);
				if (ImGui::Selectable(tagComponent ? tagComponent->name.c_str() : "Unnamed Entity", selected))
					scene->m_mainCameraEntity = entity;

				if (selected)
					ImGui::SetItemDefaultFocus();
			}

			ImGui::EndCombo();
		}
	}

	void PropertiesPanel::drawEntityProperties(Entity entity)
	{
		if (entity.hasComponents<TagComponent>())
		{
			auto& tag = entity.getComponents<TagComponent>().name;

			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			strcpy_s(buffer, sizeof(buffer), tag.c_str());
			if (ImGui::InputText("Tag", buffer, sizeof(buffer)))
			{
				tag = std::string(buffer);
			}
		}

		if (entity.hasComponents<TransformComponent>())
		{
			if (ImGui::TreeNodeEx((void*)typeid(TransformComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Transform"))
			{
				auto& transform = entity.getComponents<TransformComponent>().matrix;
				ImGui::DragFloat3("Position", glm::value_ptr(transform[3]), 0.1f);

				ImGui::TreePop();
			}
		}

		if (entity.hasComponents<CameraComponent>())
		{
			if (ImGui::TreeNodeEx((void*)typeid(CameraComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Camera"))
			{
				auto& cameraComponent = entity.getComponents<CameraComponent>();
				auto& camera = cameraComponent.camera;

				const char* projectionTypeStrings[] = { "Perspective", "Orthographic" };
				const char* currentProjectionTypeString = projectionTypeStrings[(int)camera.getProjectionType()];
				if (ImGui::BeginCombo("Projection", currentProjectionTypeString))
				{
					for (int i = 0; i < 2; ++i)
					{
						bool selected = currentProjectionTypeString == projectionTypeStrings[i];
						if (ImGui::Selectable(projectionTypeStrings[i], selected))
						{
							currentProjectionTypeString = projectionTypeStrings[i];
							camera.setProjectionType((SceneCamera::ProjectionType)i);
						}

						if (selected)
							ImGui::SetItemDefaultFocus();
					}

					ImGui::EndCombo();
				}

				if (camera.getProjectionType() == SceneCamera::ProjectionType::Perspective)
				{
					float verticalFov = glm::degrees(camera.getPerspectiveVerticalFOV());
					if (ImGui::DragFloat("Vertical FOV", &verticalFov))
						camera.setPerspectiveVerticalFOV(glm::radians(verticalFov));

					float nearClip = camera.getPerspectiveNearClip();
					if (ImGui::DragFloat("Near Clip", &nearClip))
						camera.setPerspectiveNearClip(nearClip);

					float farClip = camera.getPerspectiveFarClip();
					if (ImGui::DragFloat("Far Clip", &farClip))
						camera.setPerspectiveFarClip(farClip);
				}
				else
				{
					float orthoSize = camera.getOrthographicSize();
					if (ImGui::DragFloat("Size", &orthoSize))
						camera.setOrthographicSize(orthoSize);

					float nearClip = camera.getOrthographicNearClip();
					if (ImGui::DragFloat("Near Clip", &nearClip))
						camera.setOrthographicNearClip(nearClip);

					float farClip = camera.getOrthographicFarClip();
					if (ImGui::DragFloat("Far Clip", &farClip))
						camera.setOrthographicFarClip(farClip);
				}

				float aspectRatio = camera.getAspectRatio();
				if (ImGui::DragFloat("Aspect Ratio", &aspectRatio))
					camera.setAspectRatio(aspectRatio);

				ImGui::Checkbox("Fixed Aspect Ratio", &cameraComponent.fixedAspectRatio);

				ImGui::TreePop();
			}
		}
	}

}