#include <Minerva.h>

class Sandbox : public Minerva::Application
{
public:
	Sandbox()
	{
		MN_TRACE("bla");
	}

	~Sandbox()
	{

	}

};

Minerva::Application* Minerva::createApplication()
{
	return new Sandbox();
}