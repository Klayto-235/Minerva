#pragma once

int main(int argc, char** argv)
{
	Minerva::Log::init();

	MN_PROFILE_BEGIN_SESSION("profile-startup");
	auto app = Minerva::createApplication();
	MN_PROFILE_END_SESSION();

	MN_PROFILE_BEGIN_SESSION("profile-runtime");
	app->run();
	MN_PROFILE_END_SESSION();

	MN_PROFILE_BEGIN_SESSION("profile-shutdown");
	delete app;
	MN_PROFILE_END_SESSION();
}
