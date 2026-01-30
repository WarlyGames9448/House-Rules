add_library(Fuze SHARED
    src/main.cpp
    src/Application.cpp
    src/Log.cpp
    src/Plataform/Linux/LinuxWindow.cpp
)

add_library(glad STATIC
    vendor/glad/src/glad.c
)

target_include_directories(Fuze
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/vendor/spdlog/include
)

set_target_properties(glad PROPERTIES
    POSITION_INDEPENDENT_CODE ON
)

target_include_directories(glad
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/vendor/glad/include
)

target_compile_definitions(Fuze
    PRIVATE
        FUZE_BUILD_DLL
    PUBLIC
        FUZE_PLATFORM_LINUX
)

if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(Fuze PRIVATE -Wall -Wextra)
endif()

find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)

target_link_libraries(Fuze
    PRIVATE
        glad
        glfw
        OpenGL::GL
        Threads::Threads
        ${CMAKE_DL_LIBS}
)

target_precompile_headers(Fuze PRIVATE include/fuzepch.h)