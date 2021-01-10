#include <Minerva.h>
#include <Minerva/core/entry_point.h>

#include "EditorLayer.h"


namespace Minerva
{

	class MinervaEditor : public Application
	{
	public:
		MinervaEditor()
		{
			MN_PROFILE_FUNCTION();

			m_window = createWindow({ "Minerva Editor" });
			m_window->pushLayer(m_window->createLayer<EditorLayer>());

			enableImGui(m_window);
		}

		~MinervaEditor()
		{
			MN_PROFILE_FUNCTION();
		}

	private:
		Window* m_window;
	};

	Application* createApplication()
	{
		return new MinervaEditor();
	}

}