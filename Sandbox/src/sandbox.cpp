#include <Minerva.h>

class Sandbox : public Minerva::Application
{
public:
	Sandbox()
	{

	}

	~Sandbox()
	{

	}

};

Minerva::Application* Minerva::CreateApplication()
{
	return new Sandbox();
}