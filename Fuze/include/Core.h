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

#ifdef FUZE_ENABLE_ASSERTS
    #define FUZE_ASSERT(x, ...) {
            if (!(x) {
    FUZE_ERROR("Assertion Failed: {0}", __VA_ARGS__);
    __debugbreak();}
            }
    #define FUZE_CORE_ASSERT(x, ...) {
            if (!(x) {
    FUZE_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__);
    __debugbreak();}
            }
#else
    #define FUZE_ASSERT(x, ...)
    #define FUZE_CORE_ASSERT(x, ...)
#endif

#define BIT(X) (1 << X)