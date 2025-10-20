#include <iostream>

#include <GLFW/glfw3native.h>

#include <glad/glad.h>

#include <GLFW/glfw3.h>

int main() {

    // Initialize GLFW
    glfwInit();

    if (!glfwPlatformSupported(GLFW_PLATFORM_WAYLAND)) {
        glfwInitHint(GLFW_PLATFORM, GLFW_PLATFORM_X11);
    }

    // Defines OpenGL version (3.3 Core)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create Window
    GLFWwindow* window = glfwCreateWindow(800, 600, "Um game", NULL, NULL);

    if (window == NULL) {
        std::cerr << "Error to create window." << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    // Principal Loop
    while (!glfwWindowShouldClose(window)) {
        // Limpa a tela com uma cor
        //glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        //glClear(GL_COLOR_BUFFER_BIT);

        // Processa eventos e troca os buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    //Finish GLFW
    glfwTerminate();
    return 0;
}
