#include <iostream>

#include <glad/glad.h>

#include <GLFW/glfw3.h>

int main() {
    // Inicializa GLFW
    if (!glfwInit()) {
        std::cerr << "Falha ao inicializar GLFW!" << std::endl;
        return -1;
    }

    // Define a versão do OpenGL (3.3 Core)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Cria a janela
    GLFWwindow* window = glfwCreateWindow(800, 600, "Olá OpenGL!", NULL, NULL);
    if (window == NULL) {
        std::cerr << "Falha ao criar a janela GLFW!" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Falha ao inicializar GLAD!" << std::endl;
        return -1;
    }


    // Loop principal da aplicação
    while (!glfwWindowShouldClose(window)) {
        // Limpa a tela com uma cor
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Processa eventos e troca os buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Finaliza GLFW
    glfwTerminate();
    return 0;
}
