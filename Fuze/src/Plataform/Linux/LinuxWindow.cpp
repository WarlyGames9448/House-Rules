#include "fuzepch.h"

#include "Core.h"

#include "glad/glad.h"
#include "Plataform/Linux/LinuxWindow.h"
#include "Plataform/OpenGL/OpenGLContext.h"

#include "Events/ApplicationEvent.h"
#include "Events/JoystickEvent.h"
#include "Events/KeyEvent.h"
#include "Events/MouseEvent.h"

#include <stb_image.h>

namespace Fuze {

static bool s_GLFWInitialized = false;

static void GLFWErrorCallback(int error_code, const char* description) {
    FUZE_ERROR("[GLFW] Error: {0}, Description: {1}", error_code, description);
}

Window* Window::Create(const WindowProps& windowProps) {
    return new LinuxWindow(windowProps);
}

LinuxWindow::LinuxWindow(const WindowProps& windowProps) {
    Init(windowProps);
}

LinuxWindow::~LinuxWindow() {
    Shutdown();
}

void LinuxWindow::Init(const WindowProps& windowProps) {
    m_Data.Title = windowProps.Title;
    m_Data.Width = windowProps.Width;
    m_Data.Height = windowProps.Height;

    FUZE_CORE_INFO("Creating Window: {0}, ({1}, {2})", m_Data.Title, m_Data.Width, m_Data.Height);

    if (!s_GLFWInitialized) {
        [[maybe_unused]] int success = glfwInit();
        FUZE_CORE_ASSERT(success, "Failed to initialize GFLW!")

        s_GLFWInitialized = true;
    }

    if (!glfwPlatformSupported(GLFW_PLATFORM_WAYLAND)) {
        glfwInitHint(GLFW_PLATFORM, GLFW_PLATFORM_X11);
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    m_Window = glfwCreateWindow((int)m_Data.Width, (int)m_Data.Height, m_Data.Title.c_str(), nullptr, nullptr);
    FUZE_CORE_ASSERT(m_Window, "Failed to create GLFW window");

    m_Context = CreateScope<OpenGLContext>(m_Window);
    m_Context->Init();

    glfwSetErrorCallback(GLFWErrorCallback);
    glfwSetWindowUserPointer(m_Window, &m_Data);
    SetVSync(true);

    // Window Events ---------------------------------------------
    glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

        WindowResizedEvent event(width, height);
        data.EventCallback(event);
    });

    glfwSetFramebufferSizeCallback(m_Window, [](GLFWwindow* window, int width, int height) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

        data.Width = width;
        data.Height = height;

        FrameBufferResizedEvent event(width, height);
        data.EventCallback(event);
        glViewport(0, 0, width, height);
    });

    glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

        WindowCloseEvent event;
        data.EventCallback(event);
    });

    glfwSetWindowFocusCallback(m_Window, [](GLFWwindow* window, int focused) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

        if (focused) {
            WindowFocusEvent event;
            data.EventCallback(event);
        } else {
            WindowLostFocusEvent event;
            data.EventCallback(event);
        }
    });

    glfwSetWindowIconifyCallback(m_Window, [](GLFWwindow* window, int iconified) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

        if (iconified) {
            WindowMinimizedEvent event;
            data.EventCallback(event);
        } else {
            WindowRestoredEvent event;
            data.EventCallback(event);
        }
    });

    glfwSetWindowMaximizeCallback(m_Window, [](GLFWwindow* window, int maximized) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
        if (maximized) {
            WindowMaximizedEvent event;
            data.EventCallback(event);
        } else {
            WindowRestoredEvent event;
            data.EventCallback(event);
        }
    });

    // KeyBoard Events --------------------------------------------------
    glfwSetKeyCallback(
        m_Window,
        [](GLFWwindow* window, int key, [[maybe_unused]] int scancode, int action, [[maybe_unused]] int mods) {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

            switch (action) {
            case GLFW_PRESS: {
                KeyPressedEvent event(key, 0);
                data.EventCallback(event);
                break;
            }
            case GLFW_RELEASE: {
                KeyReleasedEvent event(key);
                data.EventCallback(event);
                break;
            }
            case GLFW_REPEAT: {
                KeyPressedEvent event(key, 1);
                data.EventCallback(event);
                break;
            }
            }
        });

    glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int codepoint) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

        KeyTypedEvent event(codepoint);
        data.EventCallback(event);
    });

    // Mouse Events --------------------------------------------------
    glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, [[maybe_unused]] int mods) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

        switch (action) {
        case GLFW_PRESS: {
            MouseButtonPressedEvent event(button);
            data.EventCallback(event);
            break;
        }
        case GLFW_RELEASE: {
            MouseButtonReleasedEvent event(button);
            data.EventCallback(event);
            break;
        }
        };
    });

    glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xoffset, double yoffset) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

        MouseScrolledEvent event((float)xoffset, (float)yoffset);
        data.EventCallback(event);
    });

    glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xpos, double ypos) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

        MouseMovedEvent event((float)xpos, (float)ypos);
        data.EventCallback(event);
    });

    // Joystick Events --------------------------------------------------
    // TODO
    glfwSetJoystickCallback([](int jid, int event) {
        if (event == GLFW_CONNECTED) {
            JoystickConnectedEvent event(jid);
        }
        if (event == GLFW_DISCONNECTED) {
            JoystickDisconnectedEvent event(jid);
        }
    });
}

void LinuxWindow::Shutdown() {
    glfwDestroyWindow(m_Window);
}

void LinuxWindow::OnUpdate() {
    glfwPollEvents();
    m_Context->SwapBuffers();
}

void LinuxWindow::SetVSync(bool enabled) {
    if (enabled) {
        glfwSwapInterval(1);
    } else {
        glfwSwapInterval(0);
    }

    m_Data.VSync = enabled;
}

Timestep LinuxWindow::GetTime() const {
    return glfwGetTime();
}

void LinuxWindow::SetTitle(const std::string& title) {
    glfwSetWindowTitle(m_Window, title.c_str());
}

void LinuxWindow::SetIcon(const std::string& filepath) {
    int width, height;
    unsigned char* data = stbi_load(filepath.c_str(), &width, &height, 0, 4);

    if (data) {
        GLFWimage images[1];
        images[0].width = width;
        images[0].height = height;
        images[0].pixels = data;

        glfwSetWindowIcon(m_Window, 1, images);

        stbi_image_free(data);
    } else {
        FUZE_CORE_ASSERT(false, "Can't Load Icon: {0}", filepath);
    }
}

}
