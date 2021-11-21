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
					m_selectedEntity.addComponent<TagComponent>("Unnamed Entity");
					m_selectedEntity.addComponent<TransformComponent>();
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

			if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) && ImGui::IsWindowHovered())
				m_state->selection = {};
		}
		ImGui::End();
	}

	void SceneHierarchyPanel::drawEntityNode(Entity entity)
	{
		bool selected = m_selectedEntity == entity &&
			(m_state->selection == EditorContext{ EditorContext::Type::Entity, &m_selectedEntity });
		const ImGuiTreeNodeFlags flags = (selected ? ImGuiTreeNodeFlags_Selected : 0)
			| ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_NoTreePushOnOpen;
		auto tagComponent = entity.tryGetComponents<TagComponent>();
		
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

		if (ImGui::IsItemClicked() || ImGui::IsItemClicked(ImGuiMouseButton_Right))
		{
			m_selectedEntity = entity;
			m_state->selection = { EditorContext::Type::Entity, &m_selectedEntity };
			selected = true;
		}

		if (selected)
		{
			if (ImGui::BeginPopupContextItem(nullptr))
			{
				if (ImGui::MenuItem("Delete Entity"))
				{
					if (selected)
					{
						m_selectedEntity = {};
						m_state->selection = {};
					}
					static_cast<Scene*>(m_state->scene.ptr)->deleteEntity(entity);
					// TODO: ERROR when deleting main camera. Shouldn't be a problem once we implement an editor camera.
				}

				ImGui::EndPopup();
			}
		}
	}

}