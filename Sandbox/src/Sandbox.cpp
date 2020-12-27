#include "ExampleLayer.h"
#include "Sandbox2D.h"

#include <Minerva.h>
#include <Minerva/core/entry_point.h>


class Sandbox : public Minerva::Application
{
public:
	Sandbox()
	{
		MN_PROFILE_FUNCTION();

		m_window = createWindow();
		m_window->pushLayer(m_window->createLayer<Sandbox2D>());

		enableImGui(m_window);
	}

	~Sandbox()
	{
		MN_PROFILE_FUNCTION();
	}

private:
	Minerva::Window* m_window;
};

Minerva::Application* Minerva::createApplication()
{
	return new Sandbox();
}