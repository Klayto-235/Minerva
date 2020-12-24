#pragma once

#ifdef MN_PLATFORM_WINDOWS

int main(int argc, char** argv)
{
	Minerva::Log::init();

	auto app = Minerva::createApplication();
	app->run();
	delete app;
}

#endif