#include "fuzepch.h"
#include <glad/glad.h>

#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

const char* vertexShaderSource = "#version 330 core\n"
                                 "layout (location = 0) in vec3 aPos;\n"
                                 "void main()\n"
                                 "{\n"
                                 "	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                                 "}\0";
const char* fragmentShaderSource = "#version 330 core\n"
                                   "out vec4 FragColor;\n"
                                   "void main()\n"
                                   "{\n"
                                   "	FragColor = vec4(0.8f, 0.3f, 0.02f, 1.0f);\n"
                                   "}\n\0";

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// Change Viewport when window resizes.
void framebuffer_size_callback([[maybe_unused]]GLFWwindow* window, int width, int height) { glViewport(0, 0, width, height); }

int init() {

    // Initialize GLFW
    glfwInit();
    if (!glfwPlatformSupported(GLFW_PLATFORM_WAYLAND)) {
        glfwInitHint(GLFW_PLATFORM, GLFW_PLATFORM_X11);
    }

    // Defines OpenGL version (3.3 Core)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // GLfloat vertices[] = {
    //     -0.5f, -0.5f, 0.0f, //
    //     0.5f,  -0.5f, 0.0f, //
    //     0.0f,  0.5f,  0.0f, //
    // };

    // Create Window
    GLFWwindow* window = glfwCreateWindow(800, 600, "House Rules", NULL, NULL);

    if (window == NULL) {
        std::cout << "Error to create window." << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    gladLoadGL();
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    glViewport(0, 0, 800, 808);
    // call the function every time the window resizes.
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

    // Principal Loop
    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        // change background color
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Finish GLFW
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
