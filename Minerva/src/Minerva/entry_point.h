#pragma once

#ifdef MN_PLATFORM_WINDOWS

int main(int argc, char** argv)
{
	Minerva::Log::init();
	MN_CORE_INFO("Log initialised!");

	auto app = Minerva::createApplication();
	app->run();
	delete app;
}

#endif