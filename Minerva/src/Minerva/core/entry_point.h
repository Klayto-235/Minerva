#pragma once

#ifdef MN_PLATFORM_WINDOWS

int main(int argc, char** argv)
{
	Minerva::Log::init();

	MN_PROFILE_BEGIN_SESSION("profile-startup.json");
	auto app = Minerva::createApplication();
	MN_PROFILE_END_SESSION();

	MN_PROFILE_BEGIN_SESSION("profile-runtime.json");
	app->run();
	MN_PROFILE_END_SESSION();

	MN_PROFILE_BEGIN_SESSION("profile-shutdown.json");
	delete app;
	MN_PROFILE_END_SESSION();
}

#endif