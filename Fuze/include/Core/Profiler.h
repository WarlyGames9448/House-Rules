#pragma once

#ifdef FUZE_PROFILE_ON
    #include <tracy/Tracy.hpp>
    #define FUZE_STACK_DEEP 15
    #define FUZE_PROFILE_FRAME() FrameMark
    #define FUZE_PROFILE_FUNCTION() ZoneScoped
    #define FUZE_PROFILE_SCOPE(name) ZoneScopedN(name)
    #define FUZE_PROFILE_ALLOC(ptr, count) TracyAllocS(ptr, count, FUZE_STACK_DEEP);
    #define FUZE_PROFILE_FREE(ptr) TracyFreeS(ptr, FUZE_STACK_DEEP);
#else
    #define FUZE_PROFILE_FRAME()
    #define FUZE_PROFILE_FUNCTION()
    #define FUZE_PROFILE_SCOPE(name)
    #define FUZE_PROFILE_ALLOC(ptr, count)
#endif

