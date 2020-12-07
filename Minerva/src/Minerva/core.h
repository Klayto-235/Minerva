#pragma once

#include "Minerva/Log.h"

#include <memory>


#ifndef MN_PLATFORM_WINDOWS
	#error Platform not supported by Minerva.
#endif

#ifdef MN_ENABLE_ASSERTS
	#define VA_ARGS(...) , ##__VA_ARGS__
	#define MN_ASSERT(x, s, ...) { if(!(x)) { MN_ERROR("Assertion Failed: " s VA_ARGS(__VA_ARGS__)); __debugbreak(); } }
	#define MN_CORE_ASSERT(x, s, ...) { if(!(x)) { MN_CORE_ERROR("Assertion Failed: " s VA_ARGS(__VA_ARGS__)); __debugbreak(); } }
#else
	#define MN_ASSERT(x, s, ...)
	#define MN_CORE_ASSERT(x, s, ...)
#endif

#define BIT(x) (1 << x)


namespace Minerva
{

	template<class T>
	using Scope = std::unique_ptr<T>;

	template<class T>
	using Ref = std::shared_ptr<T>;

}