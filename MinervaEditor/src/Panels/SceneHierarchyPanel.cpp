#include "SceneHierarchyPanel.h"

#include <imgui.h>


namespace Minerva
{

	void SceneHierarchyPanel::setScene(const Ref<Scene>& scene)
	{
		m_scene = scene;
	}

	void SceneHierarchyPanel::onImGuiRender()
	{
		ImGui::Begin("Scene Hierarchy");

		auto scene = m_scene.get();
		m_scene->m_registry.each([&](auto entityHandle)
		{
			drawEntityNode({ entityHandle , scene });
		});

		ImGui::End();
	}

	void SceneHierarchyPanel::drawEntityNode(Entity entity)
	{
		auto tagComponent = entity.tryGetComponents<TagComponent>();
		ImGuiTreeNodeFlags flags = ((m_selectedEntity == entity) ? ImGuiTreeNodeFlags_Selected : 0)
			| ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_SpanAvailWidth;
		
		bool opened;
		if (tagComponent)
			opened = ImGui::TreeNodeEx(reinterpret_cast<void*>((uint64_t)(uint32_t)entity), flags, tagComponent->name.c_str());
		else
		{
			ImGui::PushStyleColor(ImGuiCol_Text, { 1.0f, 0.7f, 0.7f, 1.0f });
			opened = ImGui::TreeNodeEx(reinterpret_cast<void*>((uint64_t)(uint32_t)entity), flags, "Unnamed Entity");
			ImGui::PopStyleColor();
		}

		if (ImGui::IsItemClicked())
			m_selectedEntity = entity;

		if (opened)
			ImGui::TreePop();
	}

}