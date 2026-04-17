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
#elif defined(FUZE_PLATFORM_LINUX)
    #include <signal.h>
    #define FUZE_DEBUGBREAK() raise(SIGTRAP)
#else
    #define FUZE_DEBUGBREAK()
#endif

#ifdef FUZE_ENABLE_ASSERTS

    #define FUZE_ASSERT(x, ...)                                              \
        {                                                                    \
            if (!(x)) {                                                      \
                FUZE_ERROR("Assertion Failed: {0}", __VA_ARGS__);            \
                FUZE_CORE_ERROR("File: {0}, Line: {1}", __FILE__, __LINE__); \
                FUZE_DEBUGBREAK();                                           \
            }                                                                \
        }
    #define FUZE_CORE_ASSERT(x, ...)                                         \
        {                                                                    \
            if (!(x)) {                                                      \
                FUZE_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__);       \
                FUZE_CORE_ERROR("File: {0}, Line: {1}", __FILE__, __LINE__); \
                FUZE_DEBUGBREAK();                                           \
            }                                                                \
        }

#else

    #define FUZE_ASSERT(x, ...)
    #define FUZE_CORE_ASSERT(x, ...)

#endif

#define BIT(X) (1 << X)

#define FUZE_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

#include <memory>

template<typename T>
using Ref = std::shared_ptr<T>;

template<typename T>
using Scope = std::unique_ptr<T>;
