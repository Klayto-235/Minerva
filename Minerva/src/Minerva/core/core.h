#pragma once

#include <memory>


#if defined MN_PLATFORM_WINDOWS
	#define MN_DEBUG_BREAK() __debugbreak()
#elif defined MN_PLATFORM_LINUX
	#include <csignal>
	#ifdef SIGTRAP
		#define MN_SIGTRAP SIGTRAP
	#else
		#define MN_SIGTRAP SIGILL
	#endif
	#define MN_DEBUG_BREAK() raise(MN_SIGTRAP)
#else
	#error Platform not supported by Minerva.
#endif

#define MN_ASSERT_FUNC_SIG __func__
#ifdef MN_ENABLE_ASSERTS
	// these probably won't work due to {}?
	#define MN_ASSERT(x, s, ...) { if(!(x)) { MN_ERROR("Assertion Failed: {}: ", MN_ASSERT_FUNC_SIG, s\
		__VA_OPT__(,) __VA_ARGS__); MN_DEBUG_BREAK(); } }
	#define MN_CORE_ASSERT(x, s, ...) { if(!(x)) { MN_CORE_ERROR("Assertion Failed: {}: ", MN_ASSERT_FUNC_SIG, s\
		__VA_OPT__(,) __VA_ARGS__); MN_DEBUG_BREAK(); } }
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