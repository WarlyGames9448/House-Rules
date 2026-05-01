#include "fuzepch.h"
#include "Plataform/OpenGL/OpenGLContext.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Fuze {

OpenGLContext::OpenGLContext(GLFWwindow* windowHandle): m_WindowHandle(windowHandle) {
}

void OpenGLContext::Init() {
    FUZE_PROFILE_FUNCTION();

    glfwMakeContextCurrent(m_WindowHandle);
    [[maybe_unused]] int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    FUZE_CORE_ASSERT(status, "Failed to initialize GLAD!")
}

void OpenGLContext::SwapBuffers() {
    FUZE_PROFILE_FUNCTION();

    glfwSwapBuffers(m_WindowHandle);
}
}
