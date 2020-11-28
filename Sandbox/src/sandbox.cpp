#include <Minerva.h>

class ExampleLayer : public Minerva::Layer
{
public:
	ExampleLayer()
		: Layer("Example")
	{
	}

	void onUpdate() override
	{
		MN_INFO("ExampleLayer::Update");
		auto [x, y] = Minerva::Application::get().getWindow().getMousePosition();
		MN_TRACE("{0}, {1}", x, y);
	}

	bool onEvent(const Minerva::Event& event) override
	{
		MN_TRACE("{0}", event);
		return true;
	}

};

class Sandbox : public Minerva::Application
{
public:
	Sandbox()
	{
		pushLayer(new ExampleLayer());
		pushOverlay(new Minerva::ImGuiLayer());
	}

	~Sandbox()
	{

	}

};

Minerva::Application* Minerva::createApplication()
{
	return new Sandbox();
}