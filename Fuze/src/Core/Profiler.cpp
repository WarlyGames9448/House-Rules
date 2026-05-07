#include "fuzepch.h"

#include "Profiler.h"

#if FUZE_PROFILE_ON
void* operator new(std::size_t count) {
    auto ptr = malloc(count);
    FUZE_PROFILE_ALLOC(ptr, count);
    return ptr;
}

void operator delete(void* ptr) noexcept {
    FUZE_PROFILE_FREE(ptr);
    free(ptr);
}

void* operator new[](std::size_t count) {
    auto ptr = malloc(count);
    FUZE_PROFILE_ALLOC(ptr, count);
    return ptr;
}

void operator delete[](void* ptr) noexcept {
    FUZE_PROFILE_FREE(ptr);
    free(ptr);
}

#endif
