#pragma once

#ifdef MN_PLATFORM_WINDOWS

int main(int argc, char** argv)
{
	Minerva::Log::init();
	MN_CORE_INFO("Initialising window system.");
	Minerva::Window::init();

	auto app = Minerva::createApplication();
	app->run();
	delete app;

	MN_CORE_INFO("Terminating window system.");
	Minerva::Window::terminate();
}

#endif