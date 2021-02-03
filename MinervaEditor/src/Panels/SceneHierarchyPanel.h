#pragma once

#include <Minerva.h>


namespace Minerva
{

	class SceneHierarchyPanel
	{
	public:
		SceneHierarchyPanel() = default;
		explicit SceneHierarchyPanel(const Ref<Scene>& scene)
			: m_scene(scene) {}

		void setScene(const Ref<Scene>& scene);

		void onImGuiRender();
	private:
		void drawEntityNode(Entity entity);

		Ref<Scene> m_scene;
		Entity m_selectedEntity;
	};

}