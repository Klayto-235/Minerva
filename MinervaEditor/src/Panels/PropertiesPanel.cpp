#include "Panels/PropertiesPanel.h"
#include "EditorLayer.h"

#include <imgui.h>

#include <glm/gtc/type_ptr.hpp>

#include <cstring>


namespace Minerva
{

	static void controlVec3(glm::vec3& vector, float resetValue = 0.0f, float speed = 0.1f)
	{
		ImGuiIO& io = ImGui::GetIO();
		auto boldFont = io.Fonts->Fonts[1];

		float frameHeight = ImGui::GetFrameHeight();
		ImVec2 buttonSize = { frameHeight + 3.0f, frameHeight };

		ImGui::PushID(&vector);
		uint32_t width = (uint32_t)(ImGui::GetContentRegionAvail().x - 3 * buttonSize.x);
		ImGui::PushItemWidth((float)(width / 3 + width % 3));
		ImGui::PushItemWidth((float)(width / 3));
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("X", buttonSize))
			vector.x = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##X", &vector.x, speed, 0.0f, 0.0f, "%.3f");

		ImGui::SameLine();
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("Y", buttonSize))
			vector.y = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Y", &vector.y, speed, 0.0f, 0.0f, "%.3f");

		ImGui::SameLine();
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("Z", buttonSize))
			vector.z = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::PopItemWidth();
		ImGui::DragFloat("##Z", &vector.z, speed, 0.0f, 0.0f, "%.3f");

		ImGui::PopItemWidth();
		ImGui::PopStyleVar();
		ImGui::PopID();
	}

	static inline void nextControl(const char* controlName)
	{
		ImGui::TableNextRow();
		ImGui::TableSetColumnIndex(0);
		ImGui::Text(controlName);
		ImGui::TableSetColumnIndex(1);
	}

	template <typename T, typename UIFunction>
	static void drawComponentNode(const char* componentName, Entity entity, UIFunction uiFunction)
	{
		static constexpr ImGuiTableFlags tableFlags = ImGuiTableFlags_BordersInnerV | ImGuiTableFlags_SizingFixedFit;
		static constexpr ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_SpanAvailWidth |
			ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed;
		static constexpr float labelColumnWidth = 100.0f;

		auto component = entity.tryGetComponents<T>();
		if (component)
		{
			const bool opened = ImGui::TreeNodeEx(reinterpret_cast<void*>(typeid(T).hash_code()),
				treeNodeFlags, componentName);

			bool deleted = false;
			if (ImGui::BeginPopupContextItem(nullptr))
			{
				if (ImGui::MenuItem("Delete Component"))
					deleted = true;

				ImGui::EndPopup();
			}

			if (opened)
			{
				if (ImGui::BeginTable("##table", 2, tableFlags))
				{
					ImGui::TableSetupColumn("", ImGuiTableColumnFlags_None, labelColumnWidth);
					ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthStretch);

					uiFunction(component);

					ImGui::EndTable();
				}

				ImGui::TreePop();
			}

			if (deleted)
				entity.removeComponents<T>();
		}
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
		if (ImGui::BeginTable("##table", 2, ImGuiTableFlags_BordersInnerV | ImGuiTableFlags_SizingFixedFit))
		{
			ImGui::TableSetupColumn("", ImGuiTableColumnFlags_None, 80.0f);
			ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthStretch);

			nextControl("Main Camera");
			ImGui::PushItemWidth(-FLT_MIN);
			auto tagComponent = scene->m_mainCameraEntity.tryGetComponents<TagComponent>();
			bool beginCombo;
			if (tagComponent)
			{
				beginCombo = ImGui::BeginCombo("", tagComponent->name.c_str());
			}
			else
			{
				ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyle().Colors[ImGuiCol_TextDisabled]);
				beginCombo = ImGui::BeginCombo("", "Unnamed Entity");
				ImGui::PopStyleColor();
			}
			if (beginCombo)
			{
				auto view = scene->m_registry.view<CameraComponent>();
				for (auto entityHandle : view)
				{
					Entity entity{ entityHandle, scene };
					tagComponent = entity.tryGetComponents<TagComponent>();

					ImGui::PushID((uint32_t)entity);
					bool selected = (entity == scene->m_mainCameraEntity);
					bool newlySelected;
					if (tagComponent)
					{
						newlySelected = ImGui::Selectable(tagComponent->name.c_str(), selected);
					}
					else
					{
						ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyle().Colors[ImGuiCol_TextDisabled]);
						newlySelected = ImGui::Selectable("Unnamed Entity", selected);
						ImGui::PopStyleColor();
					}
					if (newlySelected)
						scene->m_mainCameraEntity = entity;

					if (selected)
						ImGui::SetItemDefaultFocus();
					
					ImGui::PopID();
				}

				ImGui::EndCombo();
			}
			ImGui::PopItemWidth();

			ImGui::EndTable();
		}
	}

	void PropertiesPanel::drawEntityProperties(Entity entity)
	{
		if (ImGui::BeginPopupContextWindow(nullptr, ImGuiPopupFlags_NoOpenOverItems | ImGuiPopupFlags_MouseButtonRight))
		{
			if (ImGui::BeginMenu("Add Component"))
			{
				if (ImGui::MenuItem("Tag", nullptr, false, !entity.hasComponents<TagComponent>()))
					entity.addComponent<TagComponent>("Unnamed Entity");
				if (ImGui::MenuItem("Transform", nullptr, false, !entity.hasComponents<TransformComponent>()))
					entity.addComponent<TransformComponent>();
				if (ImGui::MenuItem("Camera", nullptr, false, !entity.hasComponents<CameraComponent>()))
					entity.addComponent<CameraComponent>();
				if (ImGui::MenuItem("Sprite Render", nullptr, false, !entity.hasComponents<SpriteRenderComponent>()))
					entity.addComponent<SpriteRenderComponent>();

				ImGui::EndMenu();
			}

			ImGui::EndPopup();
		}

		ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2{ 5.0f, 0.0f });

		drawComponentNode<TagComponent>("Tag", entity, [](auto tagComponent)
		{
			nextControl("Name");
			char buffer[256] = { 0 };
			std::strncpy(buffer, tagComponent->name.c_str(), sizeof(buffer) - 1);
			ImGui::PushItemWidth(-FLT_MIN);
			if (ImGui::InputText("", buffer, sizeof(buffer)))
				tagComponent->name = std::string(buffer);
			ImGui::PopItemWidth();
		});

		drawComponentNode<TransformComponent>("Transform", entity, [](auto transformComponent)
		{
			nextControl("Translation");
			controlVec3(transformComponent->translation);

			nextControl("Rotation");
			glm::vec3 rotation = glm::degrees(transformComponent->rotation);
			controlVec3(rotation);
			transformComponent->rotation = glm::radians(rotation);

			nextControl("Scale");
			controlVec3(transformComponent->scale, 1.0f);
		});

		drawComponentNode<CameraComponent>("Camera", entity, [](auto cameraComponent)
		{
			auto& camera = cameraComponent->camera;

			nextControl("Projection");
			ImGui::PushItemWidth(-FLT_MIN);
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

			nextControl("Fix Aspect Ratio");
			ImGui::Checkbox("##i", &cameraComponent->fixedAspectRatio);

			ImGui::PopItemWidth();
		});

		drawComponentNode<SpriteRenderComponent>("Sprite Render", entity, [](auto spriteRenderComponent)
		{
			nextControl("Color");
			ImGui::PushItemWidth(-FLT_MIN);
			ImGui::ColorEdit4("", glm::value_ptr(spriteRenderComponent->color));
			ImGui::PopItemWidth();
		});
		
		ImGui::PopStyleVar();
	}

}