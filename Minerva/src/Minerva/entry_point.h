#pragma once

#ifdef MN_PLATFORM_WINDOWS

extern Minerva::Application* Minerva::CreateApplication();

int main(int argc, char** argv)
{
	auto app = Minerva::CreateApplication();
	app->run();
	delete app;
}

#endif