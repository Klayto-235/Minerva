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

Minerva::Application* Minerva::createApplication()
{
	return new Sandbox();
}