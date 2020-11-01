#pragma once

#ifdef MN_PLATFORM_WINDOWS

// Why extern?
extern Minerva::Application* Minerva::CreateApplication();

int main(int argc, char** argv)
{
	Minerva::Log::Init();
	MN_CORE_WARN("Logger initialised!");
	int a = 6;
	MN_INFO("Hello {0}!", a);

	auto app = Minerva::CreateApplication();
	app->run();
	delete app;
}

#endif