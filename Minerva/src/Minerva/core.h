#pragma once

#ifdef MN_PLATFORM_WINDOWS
	#ifdef MN_BUILD_DLL
		#define MINERVA_API __declspec(dllexport)
	#else
		#define MINERVA_API __declspec(dllimport)
	#endif
#else
	#error Platform not supported by Minerva.
#endif
