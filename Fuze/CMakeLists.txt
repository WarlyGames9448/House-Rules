add_library(Fuze SHARED
    src/Application.cpp
    src/Layer.cpp
    src/LayerStack.cpp
    src/Log.cpp

    src/ImGui/ImGuiLayer.cpp

    src/Plataform/Linux/LinuxInput.cpp
    src/Plataform/Linux/LinuxWindow.cpp
    src/Plataform/OpenGL/OpenGLBuffer.cpp
    src/Plataform/OpenGL/OpenGLContext.cpp
    src/Plataform/OpenGL/OpenGLRendererAPI.cpp
    src/Plataform/OpenGL/OpenGLShader.cpp
    src/Plataform/OpenGL/OpenGLTexture.cpp
    src/Plataform/OpenGL/OpenGLVertexArray.cpp


    src/Renderer/Buffer.cpp
    src/Renderer/OrthographicCamera.cpp
    src/Renderer/OrthographicCameraController.cpp
    src/Renderer/Renderer.cpp
    src/Renderer/RendererAPI.cpp
    src/Renderer/RendererCommand.cpp
    src/Renderer/Shader.cpp
    src/Renderer/Texture.cpp
    src/Renderer/VertexArray.cpp
)

target_include_directories(Fuze
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/spdlog/include
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/glfw/include
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/glm/glm
)

# ═══════════════════════════════════════════════════════════
# STB
# ═══════════════════════════════════════════════════════════

add_library(stb STATIC
    vendor/stb/stb_image.cpp
)

target_include_directories(stb
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/stb
)

# ═══════════════════════════════════════════════════════════
# GLAD
# ═══════════════════════════════════════════════════════════

add_library(glad STATIC
    vendor/glad/src/glad.c
)

target_include_directories(glad
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/glad/include
)

# ═══════════════════════════════════════════════════════════
# ImGui
# ═══════════════════════════════════════════════════════════

add_library(imgui STATIC
    vendor/imgui/imgui.cpp
    vendor/imgui/imgui_draw.cpp
    vendor/imgui/imgui_tables.cpp
    vendor/imgui/imgui_widgets.cpp
    vendor/imgui/backends/imgui_impl_glfw.cpp
    vendor/imgui/backends/imgui_impl_opengl3.cpp
    vendor/imgui/imgui_demo.cpp
)

target_include_directories(imgui PUBLIC
    vendor/imgui
    vendor/imgui/backends
)

target_link_libraries(imgui
    PRIVATE glfw
)

target_compile_definitions(imgui PRIVATE
    IMGUI_IMPL_OPENGL_LOADER_GLAD
)

# ═══════════════════════════════════════════════════════════
# OTHERS
# ═══════════════════════════════════════════════════════════

target_compile_definitions(Fuze
    PRIVATE
    FUZE_BUILD_DLL
    PUBLIC
    FUZE_PLATFORM_LINUX
    FUZE_ENABLE_ASSERTS
)

if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(Fuze PRIVATE -Wall -Wextra)
endif()

find_package(OpenGL REQUIRED)

target_link_libraries(Fuze
    PUBLIC
        glad
        glfw
        imgui
        OpenGL::GL
        stb
        Threads::Threads
        ${CMAKE_DL_LIBS}
)

target_precompile_headers(Fuze PRIVATE include/fuzepch.h)
