cmake_minimum_required(VERSION 3.10)

project(Um-Game)

cmake_policy(SET CMP0072 NEW)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)

file(GLOB_RECURSE MY_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

add_executable(um_game ${MY_SOURCES} libs/glad/src/glad.c)

target_include_directories(um_game PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/libs/glad/include)

target_sources(um_game PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/libs/glad/src/glad.c)

target_link_libraries(um_game PRIVATE
    glfw
    OpenGL::GL
)
