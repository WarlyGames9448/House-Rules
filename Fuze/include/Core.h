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