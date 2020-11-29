#include <Minerva.h>
#include <imgui.h>

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
		const auto& window = Minerva::Application::get().getWindow();
		if (window.isMouseButtonPressed(Minerva::MouseButton::ButtonLeft))
		{
			auto [x, y] = window.getMousePosition();
			MN_TRACE("{0}, {1}", x, y);
		}
	}

	bool onEvent(const Minerva::Event& event) override
	{
		MN_TRACE("{0}", event);
		return true;
	}

	void onImGuiRender() override
	{
		ImGui::Begin("Test");
		ImGui::Text("Hello World");
		ImGui::End();
	}

};

class Sandbox : public Minerva::Application
{
public:
	Sandbox()
	{
		pushLayer(new ExampleLayer());
	}

	~Sandbox()
	{

	}
};

Minerva::Application* Minerva::createApplication()
{
	return new Sandbox();
}