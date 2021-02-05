#pragma once

#include <Minerva.h>


namespace Minerva
{
	struct EditorState;

	class PropertiesPanel
	{
	public:
		explicit PropertiesPanel(EditorState* state)
			: m_state(state) {}

		void onImGuiRender();
	private:
		void drawSceneProperties(Scene* scene);
		void drawEntityProperties(Entity entity);

		EditorState* const m_state;
	};

}