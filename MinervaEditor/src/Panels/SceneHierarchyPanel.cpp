#include "Panels/SceneHierarchyPanel.h"
#include "EditorLayer.h"

#include <imgui.h>


namespace Minerva
{

	void SceneHierarchyPanel::onImGuiRender()
	{
		if (ImGui::Begin("Scene Hierarchy"))
		{
			const ImGuiTreeNodeFlags flags = ((m_state->selection == m_state->scene) ? ImGuiTreeNodeFlags_Selected : 0)
				| ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_OpenOnDoubleClick
				| ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
			bool opened = ImGui::TreeNodeEx(m_state->scene.ptr, flags, "Scene");
			if (ImGui::IsItemClicked() || ImGui::IsItemClicked(ImGuiMouseButton_Right))
			{
				m_selectedEntity = {};
				m_state->selection = m_state->scene;
			}

			if (ImGui::BeginPopupContextItem(nullptr))
			{
				if (ImGui::MenuItem("New Entity"))
				{
					m_selectedEntity = static_cast<Scene*>(m_state->scene.ptr)->newEntity();
					m_state->selection = { EditorContext::Type::Entity, &m_selectedEntity };
				}

				ImGui::EndPopup();
			}

			if (opened) {
				auto scene = static_cast<Scene*>(m_state->scene.ptr);
				scene->m_registry.each([=](auto entityHandle)
				{
					drawEntityNode({ entityHandle , scene });
				});

				ImGui::TreePop();
			}

			if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) && ImGui::IsWindowHovered() && !ImGui::IsAnyItemHovered())
				m_state->selection = {};
		}
		ImGui::End();
	}

	void SceneHierarchyPanel::drawEntityNode(Entity entity)
	{
		const bool selected = m_selectedEntity == entity &&
			(m_state->selection == EditorContext{ EditorContext::Type::Entity, &m_selectedEntity });
		const ImGuiTreeNodeFlags flags = (selected ? ImGuiTreeNodeFlags_Selected : 0)
			| ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_NoTreePushOnOpen
			| ImGuiTreeNodeFlags_AllowItemOverlap;
		auto tagComponent = entity.tryGetComponents<TagComponent>();
		
		float fontSize = ImGui::GetFontSize();
		ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();
		if (tagComponent)
		{
			ImGui::TreeNodeEx(reinterpret_cast<void*>((uint64_t)(uint32_t)entity), flags, tagComponent->name.c_str());
		}
		else
		{
			ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyle().Colors[ImGuiCol_TextDisabled]);
			ImGui::TreeNodeEx(reinterpret_cast<void*>((uint64_t)(uint32_t)entity), flags, "Unnamed Entity");
			ImGui::PopStyleColor();
		}
		
		const bool nodeClicked = ImGui::IsItemClicked();

		ImGui::SameLine(contentRegionAvailable.x);
		ImGui::PushID((uint32_t)entity);
		if (ImGui::Button("x", ImVec2{ fontSize, fontSize }))
		{
			if (selected)
			{
				m_selectedEntity = {};
				m_state->selection = {};
			}
			static_cast<Scene*>(m_state->scene.ptr)->deleteEntity(entity);
			// TODO: ERROR when deleting main camera. Shouldn't be a problem once we implement an editor camera.
		}
		ImGui::PopID();

		if (nodeClicked && !ImGui::IsItemClicked())
		{
			m_selectedEntity = entity;
			m_state->selection = { EditorContext::Type::Entity, &m_selectedEntity };
		}
	}

}