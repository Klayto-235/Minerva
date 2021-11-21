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
			enableImGui(m_window);

			m_window->pushLayer(m_window->newLayer<EditorLayer>());

			ImGuiIO& io = ImGui::GetIO();
			io.FontDefault = io.Fonts->AddFontFromFileTTF("assets/fonts/roboto/Roboto-Regular.ttf", 15.0);
			io.Fonts->AddFontFromFileTTF("assets/fonts/roboto/Roboto-Bold.ttf", 15.0);
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