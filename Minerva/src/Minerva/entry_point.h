#pragma once

#ifdef MN_PLATFORM_WINDOWS

int main(int argc, char** argv)
{
	Minerva::Log::Init();
	MN_CORE_INFO("Log initialised!");

	auto app = Minerva::CreateApplication();
	app->run();
	delete app;
}

#endif