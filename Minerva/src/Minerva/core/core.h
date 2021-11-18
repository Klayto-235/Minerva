#pragma once

#include <memory>


#ifndef MN_PLATFORM_WINDOWS
	#error Platform not supported by Minerva.
#endif

#define MN_ASSERT_FUNC_SIG __FUNCTION__
#define MN_DEBUG_BREAK() __debugbreak() // TODO: make portable

#ifdef MN_ENABLE_ASSERTS
	#define VA_ARGS(...) , ##__VA_ARGS__
	#define MN_ASSERT(x, s, ...) { if(!(x)) { MN_ERROR("Assertion Failed: " MN_ASSERT_FUNC_SIG ": " s\
		VA_ARGS(__VA_ARGS__)); MN_DEBUG_BREAK(); } }
	#define MN_CORE_ASSERT(x, s, ...) { if(!(x)) { MN_CORE_ERROR("Assertion Failed: " MN_ASSERT_FUNC_SIG ": " s\
		VA_ARGS(__VA_ARGS__)); MN_DEBUG_BREAK(); } }
#else
	#define MN_ASSERT(x, s, ...)
	#define MN_CORE_ASSERT(x, s, ...)
#endif

#define BIT(x) (1 << x)

#define TOKENPASTE(x,y) x##y
#define TOKENPASTE2(x,y) TOKENPASTE(x,y)
#define UNIQUE(x) TOKENPASTE2(x, __COUNTER__)
#define LINE(x) TOKENPASTE2(x, __LINE__)

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