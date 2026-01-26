add_library(Fuze SHARED
    src/main.cpp
    src/Application.cpp
    vendor/glad/src/glad.c)

target_include_directories(Fuze
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/vendor/glad/include
)

target_compile_definitions(Fuze
    PRIVATE
        FUZE_BUILD_DLL
    PUBLIC
        FUZE_PLATFORM_LINUX
)

target_link_libraries(Fuze PRIVATE Threads::Threads ${CMAKE_DL_LIBS})

if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(Fuze PRIVATE -Wall -Wextra)
endif()

find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)

target_link_libraries(Fuze
    PRIVATE
        glfw
        OpenGL::GL
)