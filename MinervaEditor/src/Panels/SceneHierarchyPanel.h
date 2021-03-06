#pragma once

#include <Minerva.h>


namespace Minerva
{
	struct EditorState;

	class SceneHierarchyPanel
	{
	public:
		explicit SceneHierarchyPanel(EditorState* state)
			: m_state(state) {}

		void onImGuiRender();
	private:
		void drawEntityNode(Entity entity);

		EditorState* const m_state;
		Entity m_selectedEntity{};
	};

}