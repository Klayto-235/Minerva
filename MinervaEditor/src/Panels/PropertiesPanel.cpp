#include "Panels/PropertiesPanel.h"
#include "EditorLayer.h"

#include <imgui.h>

#include <glm/gtc/type_ptr.hpp>


namespace Minerva
{

	static void controlVec3(glm::vec3& vector, float resetValue = 0.0f, float speed = 1.0f)
	{
		float frameHeight = ImGui::GetFrameHeight();
		ImVec2 buttonSize = { frameHeight + 3.0f, frameHeight };

		ImGui::PushID(&vector);
		uint32_t width = (uint32_t)(ImGui::CalcItemWidth() / 0.65f - 3 * buttonSize.x);
		ImGui::PushItemWidth((float)(width / 3 + width % 3));
		ImGui::PushItemWidth((float)(width / 3));
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		if (ImGui::Button("X", buttonSize))
			vector.x = resetValue;
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##X", &vector.x, 0.1f, 0.0f, 0.0f, "%.2f");

		ImGui::SameLine();
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		if (ImGui::Button("Y", buttonSize))
			vector.y = resetValue;
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Y", &vector.y, 0.1f, 0.0f, 0.0f, "%.2f");

		ImGui::SameLine();
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		if (ImGui::Button("Z", buttonSize))
			vector.z = resetValue;
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::PopItemWidth();
		ImGui::DragFloat("##Z", &vector.z, 0.1f, 0.0f, 0.0f, "%.2f");

		ImGui::PopItemWidth();
		ImGui::PopStyleVar();
		ImGui::PopID();
	}

	static void nextControl(const char* controlName)
	{
		ImGui::TableNextRow();
		ImGui::TableSetColumnIndex(0);
		ImGui::Text(controlName);
		ImGui::TableSetColumnIndex(1);
	}

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

				ImGui::PushID((uint32_t)entity);
				bool selected = (entity == scene->m_mainCameraEntity);
				if (ImGui::Selectable(tagComponent ? tagComponent->name.c_str() : "Unnamed Entity", selected))
					scene->m_mainCameraEntity = entity;

				if (selected)
					ImGui::SetItemDefaultFocus();
				ImGui::PopID();
			}

			ImGui::EndCombo();
		}
	}

	void PropertiesPanel::drawEntityProperties(Entity entity)
	{
		static constexpr ImGuiTableFlags tableFlags = ImGuiTableFlags_BordersInnerV | ImGuiTableFlags_SizingFixedFit;
		static constexpr ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_SpanAvailWidth |
			ImGuiTreeNodeFlags_DefaultOpen;
		ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2{ 5.0f, 0.0f });

		auto tagComponent = entity.tryGetComponents<TagComponent>();
		if (tagComponent)
		{
			if (ImGui::TreeNodeEx(reinterpret_cast<void*>(typeid(TagComponent).hash_code()), treeNodeFlags, "Tag"))
			{
				if (ImGui::BeginTable("##table", 2, tableFlags))
				{
					ImGui::TableSetupColumn("", ImGuiTableColumnFlags_None, 80.0f);
					ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthStretch);

					nextControl("Name");
					char buffer[256] = { 0 };
					strcpy_s(buffer, sizeof(buffer), tagComponent->name.c_str());
					ImGui::PushItemWidth(ImGui::CalcItemWidth() / 0.65f);
					if (ImGui::InputText("", buffer, sizeof(buffer)))
						tagComponent->name = std::string(buffer);
					ImGui::PopItemWidth();

					ImGui::EndTable();
				}

				ImGui::TreePop();
			}
		}

		auto transformComponent = entity.tryGetComponents<TransformComponent>();
		if (transformComponent)
		{
			if (ImGui::TreeNodeEx(reinterpret_cast<void*>(typeid(TransformComponent).hash_code()), treeNodeFlags, "Transform"))
			{
				if (ImGui::BeginTable("##table", 2, tableFlags))
				{
					ImGui::TableSetupColumn("", ImGuiTableColumnFlags_None, 80.0f);
					ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthStretch);

					nextControl("Translation");
					controlVec3(transformComponent->translation);

					nextControl("Rotation");
					glm::vec3 rotation = glm::degrees(transformComponent->rotation);
					controlVec3(rotation);
					transformComponent->rotation = glm::radians(rotation);

					nextControl("Scale");
					controlVec3(transformComponent->scale, 1.0f);

					ImGui::EndTable();
				}

				ImGui::TreePop();
			}
		}

		auto cameraComponent = entity.tryGetComponents<CameraComponent>();
		if (cameraComponent)
		{
			if (ImGui::TreeNodeEx(reinterpret_cast<void*>(typeid(CameraComponent).hash_code()), treeNodeFlags, "Camera"))
			{
				if (ImGui::BeginTable("##table", 2, tableFlags))
				{
					ImGui::TableSetupColumn("", ImGuiTableColumnFlags_None, 130.0f);
					ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthStretch);

					auto& camera = cameraComponent->camera;

					nextControl("Projection");
					ImGui::PushItemWidth(ImGui::CalcItemWidth() / 0.65f);
					const char* projectionTypeStrings[] = { "Perspective", "Orthographic" };
					const char* currentProjectionTypeString = projectionTypeStrings[(int)camera.getProjectionType()];
					if (ImGui::BeginCombo("##a", currentProjectionTypeString))
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
						nextControl("Vertical FOV");
						float verticalFov = glm::degrees(camera.getPerspectiveVerticalFOV());
						if (ImGui::DragFloat("##b", &verticalFov, 0.1f))
							camera.setPerspectiveVerticalFOV(glm::radians(verticalFov));

						nextControl("Near Clip");
						float nearClip = camera.getPerspectiveNearClip();
						if (ImGui::DragFloat("##c", &nearClip, 0.1f))
							camera.setPerspectiveNearClip(nearClip);

						nextControl("Far Clip");
						float farClip = camera.getPerspectiveFarClip();
						if (ImGui::DragFloat("##d", &farClip, 0.1f))
							camera.setPerspectiveFarClip(farClip);
					}
					else
					{
						nextControl("Size");
						float orthoSize = camera.getOrthographicSize();
						if (ImGui::DragFloat("##e", &orthoSize, 0.1f))
							camera.setOrthographicSize(orthoSize);

						nextControl("Near Clip");
						float nearClip = camera.getOrthographicNearClip();
						if (ImGui::DragFloat("##f", &nearClip, 0.1f))
							camera.setOrthographicNearClip(nearClip);

						nextControl("Far Clip");
						float farClip = camera.getOrthographicFarClip();
						if (ImGui::DragFloat("##g", &farClip, 0.1f))
							camera.setOrthographicFarClip(farClip);
					}

					nextControl("Aspect Ratio");
					float aspectRatio = camera.getAspectRatio();
					if (ImGui::DragFloat("##h", &aspectRatio, 0.1f))
						camera.setAspectRatio(aspectRatio);

					nextControl("Fixed Aspect Ratio");
					ImGui::Checkbox("##i", &cameraComponent->fixedAspectRatio);

					ImGui::PopItemWidth();
					ImGui::EndTable();
				}

				ImGui::TreePop();
			}
		}
		
		auto spriteRenderComponent = entity.tryGetComponents<SpriteRenderComponent>();
		if (spriteRenderComponent)
		{
			if (ImGui::TreeNodeEx(reinterpret_cast<void*>(typeid(SpriteRenderComponent).hash_code()), treeNodeFlags, "Sprite Render"))
			{
				if (ImGui::BeginTable("##table", 2, tableFlags))
				{
					ImGui::TableSetupColumn("", ImGuiTableColumnFlags_None, 80.0f);
					ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthStretch);

					ImGui::TableNextRow();
					ImGui::TableSetColumnIndex(0);
					ImGui::Text("Color");
					ImGui::TableSetColumnIndex(1);
					ImGui::PushItemWidth(ImGui::CalcItemWidth() / 0.65f);
					ImGui::ColorEdit4("", glm::value_ptr(spriteRenderComponent->color));
					ImGui::PopItemWidth();

					ImGui::EndTable();
				}

				ImGui::TreePop();
			}
		}

		ImGui::PopStyleVar();
	}

}