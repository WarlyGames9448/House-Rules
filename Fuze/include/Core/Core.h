#pragma once

#ifdef FUZE_PLATFORM_LINUX
    #ifdef FUZE_BUILD_DLL
        #define FUZE_API __attribute__((visibility("default")))
    #else
        #define FUZE_API
    #endif
#else
static_assert("Fuze only works on Linux.");
#endif

#ifdef FUZE_PLATFORM_WINDOWS
    #define FUZE_DEBUGBREAK() __debugbreak()
    #define NOMINMAX
    #include <Windows.h>
#elif defined(FUZE_PLATFORM_LINUX)
    #include <signal.h>
    #include <unistd.h>
    #include <limits.h>
    #define FUZE_DEBUGBREAK() raise(SIGTRAP)
#elif defined(FUZE_PLATFORM_MACOS)
    #include <mach-o/dyld.h>
    #include <limits.h>
#else
    #define FUZE_DEBUGBREAK()
#endif

#ifdef FUZE_ENABLE_ASSERTS

    #if defined(__GNUC__) || defined(__clang__)
        #define FUZE_FUNC_SIG __PRETTY_FUNCTION__
    #elif defined(_MSC_VER)
        #define FUZE_FUNC_SIG __FUNCSIG__
    #else
        #define FUZE_FUNC_SIG __func__
    #endif

    #define FUZE_ASSERT(x, ...)                               \
        do {                                                  \
            if (!(x)) {                                       \
                FUZE_CORE_ERROR("{0}", FUZE_FUNC_SIG);        \
                FUZE_ERROR("Assertion Failed: " __VA_ARGS__); \
                FUZE_DEBUGBREAK();                            \
            }                                                 \
        } while (0);

    #define FUZE_CORE_ASSERT(x, ...)                               \
        do {                                                       \
            if (!(x)) {                                            \
                FUZE_CORE_ERROR("{0}", FUZE_FUNC_SIG);             \
                FUZE_CORE_ERROR("Assertion Failed: " __VA_ARGS__); \
                FUZE_DEBUGBREAK();                                 \
            }                                                      \
        } while (0);

#else

    #define FUZE_ASSERT(x, ...)
    #define FUZE_CORE_ASSERT(x, ...)

#endif

#define BIT(X) (1 << X)

#define FUZE_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

#include <memory>

template <typename T> using Ref = std::shared_ptr<T>;
template <typename T, typename... Args> constexpr Ref<T> CreateRef(Args&&... args) {
    return std::make_shared<T>(std::forward<Args>(args)...);
}

template <typename T> using Scope = std::unique_ptr<T>;
template <typename T, typename... Args> constexpr Scope<T> CreateScope(Args&&... args) {
    return std::make_unique<T>(std::forward<Args>(args)...);
}
