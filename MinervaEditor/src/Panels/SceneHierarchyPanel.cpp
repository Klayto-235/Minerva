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
			if (ImGui::IsItemClicked())
			{
				m_selectedEntity = {};
				m_state->selection = m_state->scene;
			}

			if (opened) {
				auto scene = static_cast<Scene*>(m_state->scene.ptr);
				scene->m_registry.each([=](auto entityHandle)
				{
					drawEntityNode({ entityHandle , &scene->m_registry });
				});

				ImGui::TreePop();
			}

			if (ImGui::IsMouseDown(ImGuiMouseButton_Left) && ImGui::IsWindowHovered())
				m_state->selection = {};
		}
		ImGui::End();
	}

	void SceneHierarchyPanel::drawEntityNode(Entity entity)
	{
		const bool selected = m_selectedEntity == entity &&
			(m_state->selection == EditorContext{ EditorContext::Type::Entity, &m_selectedEntity });
		const ImGuiTreeNodeFlags flags = (selected ? ImGuiTreeNodeFlags_Selected : 0)
			| ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_NoTreePushOnOpen;
		auto tagComponent = entity.tryGetComponents<TagComponent>();
		
		ImGui::TreeNodeEx(reinterpret_cast<void*>((uint64_t)(uint32_t)entity), flags,
			tagComponent ? tagComponent->name.c_str() : "Unnamed Entity");

		if (ImGui::IsItemClicked())
		{
			m_selectedEntity = entity;
			m_state->selection = { EditorContext::Type::Entity, &m_selectedEntity };
		}
	}

}