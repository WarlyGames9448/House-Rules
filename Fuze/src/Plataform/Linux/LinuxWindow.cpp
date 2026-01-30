#include "fuzepch.h"

#include "Core.h"
#include "Log.h"
#include <GLFW/glfw3.h>
#include "Plataform/Linux/LinuxWindow.h"

namespace Fuze {

    static bool s_GLFWInitialized = false;

    Window* Window::Create(const WindowProps& windowProps) { return new LinuxWindow(windowProps); }

    LinuxWindow::LinuxWindow(const WindowProps& windowProps) { Init(windowProps); }

    LinuxWindow::~LinuxWindow() { Shutdown(); }

    void LinuxWindow::Init(const WindowProps& windowProps) {
        m_Data.Title = windowProps.Title;
        m_Data.Width = windowProps.Width;
        m_Data.Height = windowProps.Height;

        FUZE_CORE_INFO("Creating Window: {0}, ({1}, {2})", m_Data.Title, m_Data.Width, m_Data.Height);

        if (!s_GLFWInitialized) {
            [[maybe_unused]] int success = glfwInit();
            FUZE_CORE_ASSERT(success, "Could not initialize GFLW!")

            s_GLFWInitialized = true;
        }

        if (!glfwPlatformSupported(GLFW_PLATFORM_WAYLAND)) {
            glfwInitHint(GLFW_PLATFORM, GLFW_PLATFORM_X11);
        }

        m_Window = glfwCreateWindow((int)m_Data.Width, (int)m_Data.Height, m_Data.Title.c_str(), nullptr, nullptr);
        glfwMakeContextCurrent(m_Window);
        glfwSetWindowUserPointer(m_Window, &m_Data);
        SetVSync(true);
    }

    void LinuxWindow::Shutdown() { glfwDestroyWindow(m_Window); }

    void LinuxWindow::OnUpdate() {
        glfwPollEvents();
        glfwSwapBuffers(m_Window);
    }

    void LinuxWindow::SetVSync(bool enabled) {
        if (enabled) {
            glfwSwapInterval(1);
        } else {
            glfwSwapInterval(0);
        }

        m_Data.VSync = enabled;
    }
}