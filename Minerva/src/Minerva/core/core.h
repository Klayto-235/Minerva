#pragma once

#include "Minerva/core/Log.h"

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

	template<typename T>
	using Scope = std::unique_ptr<T>;
	template<typename T, typename ...Args>
	constexpr Scope<T> createScope(Args&&... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Ref = std::shared_ptr<T>;
	template<typename T, typename ...Args>
	constexpr Ref<T> createRef(Args&&... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Ptr = std::weak_ptr<T>;
}