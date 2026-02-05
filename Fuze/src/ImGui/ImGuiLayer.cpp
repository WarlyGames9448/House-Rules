#include "fuzepch.h"

#include "ImGui/ImGuiLayer.h"

#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include "GLFW/glfw3.h"

#include "Application.h"

namespace Fuze {

    // clang-format off
    ImGuiKey GLFWKeyToImGuiKey(int key){
        switch (key)
        {
            case GLFW_KEY_TAB:              return ImGuiKey_Tab;
            case GLFW_KEY_LEFT:             return ImGuiKey_LeftArrow;
            case GLFW_KEY_RIGHT:            return ImGuiKey_RightArrow;
            case GLFW_KEY_UP:               return ImGuiKey_UpArrow;
            case GLFW_KEY_DOWN:             return ImGuiKey_DownArrow;
            case GLFW_KEY_PAGE_UP:          return ImGuiKey_PageUp;
            case GLFW_KEY_PAGE_DOWN:        return ImGuiKey_PageDown;
            case GLFW_KEY_HOME:             return ImGuiKey_Home;
            case GLFW_KEY_END:              return ImGuiKey_End;
            case GLFW_KEY_INSERT:           return ImGuiKey_Insert;
            case GLFW_KEY_DELETE:           return ImGuiKey_Delete;
            case GLFW_KEY_BACKSPACE:        return ImGuiKey_Backspace;
            case GLFW_KEY_SPACE:            return ImGuiKey_Space;
            case GLFW_KEY_ENTER:            return ImGuiKey_Enter;
            case GLFW_KEY_ESCAPE:           return ImGuiKey_Escape;
            case GLFW_KEY_APOSTROPHE:       return ImGuiKey_Apostrophe;
            case GLFW_KEY_COMMA:            return ImGuiKey_Comma;
            case GLFW_KEY_MINUS:            return ImGuiKey_Minus;
            case GLFW_KEY_PERIOD:           return ImGuiKey_Period;
            case GLFW_KEY_SLASH:            return ImGuiKey_Slash;
            case GLFW_KEY_SEMICOLON:        return ImGuiKey_Semicolon;
            case GLFW_KEY_EQUAL:            return ImGuiKey_Equal;
            case GLFW_KEY_LEFT_BRACKET:     return ImGuiKey_LeftBracket;
            case GLFW_KEY_BACKSLASH:        return ImGuiKey_Backslash;
            case GLFW_KEY_RIGHT_BRACKET:    return ImGuiKey_RightBracket;
            case GLFW_KEY_GRAVE_ACCENT:     return ImGuiKey_GraveAccent;
            case GLFW_KEY_CAPS_LOCK:        return ImGuiKey_CapsLock;
            case GLFW_KEY_SCROLL_LOCK:      return ImGuiKey_ScrollLock;
            case GLFW_KEY_NUM_LOCK:         return ImGuiKey_NumLock;
            case GLFW_KEY_PRINT_SCREEN:     return ImGuiKey_PrintScreen;
            case GLFW_KEY_PAUSE:            return ImGuiKey_Pause;

            // Function keys
            case GLFW_KEY_F1:  return ImGuiKey_F1;
            case GLFW_KEY_F2:  return ImGuiKey_F2;
            case GLFW_KEY_F3:  return ImGuiKey_F3;
            case GLFW_KEY_F4:  return ImGuiKey_F4;
            case GLFW_KEY_F5:  return ImGuiKey_F5;
            case GLFW_KEY_F6:  return ImGuiKey_F6;
            case GLFW_KEY_F7:  return ImGuiKey_F7;
            case GLFW_KEY_F8:  return ImGuiKey_F8;
            case GLFW_KEY_F9:  return ImGuiKey_F9;
            case GLFW_KEY_F10: return ImGuiKey_F10;
            case GLFW_KEY_F11: return ImGuiKey_F11;
            case GLFW_KEY_F12: return ImGuiKey_F12;

            // Letters
            case GLFW_KEY_A: return ImGuiKey_A;
            case GLFW_KEY_B: return ImGuiKey_B;
            case GLFW_KEY_C: return ImGuiKey_C;
            case GLFW_KEY_D: return ImGuiKey_D;
            case GLFW_KEY_E: return ImGuiKey_E;
            case GLFW_KEY_F: return ImGuiKey_F;
            case GLFW_KEY_G: return ImGuiKey_G;
            case GLFW_KEY_H: return ImGuiKey_H;
            case GLFW_KEY_I: return ImGuiKey_I;
            case GLFW_KEY_J: return ImGuiKey_J;
            case GLFW_KEY_K: return ImGuiKey_K;
            case GLFW_KEY_L: return ImGuiKey_L;
            case GLFW_KEY_M: return ImGuiKey_M;
            case GLFW_KEY_N: return ImGuiKey_N;
            case GLFW_KEY_O: return ImGuiKey_O;
            case GLFW_KEY_P: return ImGuiKey_P;
            case GLFW_KEY_Q: return ImGuiKey_Q;
            case GLFW_KEY_R: return ImGuiKey_R;
            case GLFW_KEY_S: return ImGuiKey_S;
            case GLFW_KEY_T: return ImGuiKey_T;
            case GLFW_KEY_U: return ImGuiKey_U;
            case GLFW_KEY_V: return ImGuiKey_V;
            case GLFW_KEY_W: return ImGuiKey_W;
            case GLFW_KEY_X: return ImGuiKey_X;
            case GLFW_KEY_Y: return ImGuiKey_Y;
            case GLFW_KEY_Z: return ImGuiKey_Z;

            // Numbers
            case GLFW_KEY_0: return ImGuiKey_0;
            case GLFW_KEY_1: return ImGuiKey_1;
            case GLFW_KEY_2: return ImGuiKey_2;
            case GLFW_KEY_3: return ImGuiKey_3;
            case GLFW_KEY_4: return ImGuiKey_4;
            case GLFW_KEY_5: return ImGuiKey_5;
            case GLFW_KEY_6: return ImGuiKey_6;
            case GLFW_KEY_7: return ImGuiKey_7;
            case GLFW_KEY_8: return ImGuiKey_8;
            case GLFW_KEY_9: return ImGuiKey_9;
        }
            return ImGuiKey_None;
    }
    // clang-format on

    ImGuiLayer::ImGuiLayer() : Layer("ImGuiLayer") {}

    ImGuiLayer::~ImGuiLayer() {}

    void ImGuiLayer::OnAttach() {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        (void)io;

        ImGui::StyleColorsDark();

        /* static Application& app = Application::Get();
        GLFWwindow* window = static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());
        ImGui_ImplGlfw_InitForOpenGL(window, true); */

        ImGui_ImplOpenGL3_Init("#version 410");
    }

    void ImGuiLayer::OnDetach() {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    void ImGuiLayer::OnUpdate() {

        ImGuiIO& io = ImGui::GetIO();
        Application& app = Application::Get();
        Window& window = app.GetWindow();
        io.DisplaySize = ImVec2(window.GetWidth(), window.GetHeight());

        float time = (float)glfwGetTime();
        io.DeltaTime = m_Time > 0.0f ? (time - m_Time) : (1.0f / 60.0f);
        m_Time = time;

        ImGui_ImplOpenGL3_NewFrame();
        ImGui::NewFrame();

        static bool show = true;
        ImGui::ShowDemoWindow(&show);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    void ImGuiLayer::OnEvent(Event& event) {
        EventDispatcher dispatcher(event);
        dispatcher.Dispatch<MouseButtonPressedEvent>(FUZE_BIND_EVENT_FN(ImGuiLayer::OnMouseButtonPressedEvent));
        dispatcher.Dispatch<MouseButtonReleasedEvent>(FUZE_BIND_EVENT_FN(ImGuiLayer::OnMouseButtonReleasedEvent));
        dispatcher.Dispatch<MouseMovedEvent>(FUZE_BIND_EVENT_FN(ImGuiLayer::OnMouseMovedEvent));
        dispatcher.Dispatch<MouseScrolledEvent>(FUZE_BIND_EVENT_FN(ImGuiLayer::OnMouseScrolledEvent));
        dispatcher.Dispatch<KeyPressedEvent>(FUZE_BIND_EVENT_FN(ImGuiLayer::OnKeyPressedEvent));
        dispatcher.Dispatch<KeyReleasedEvent>(FUZE_BIND_EVENT_FN(ImGuiLayer::OnKeyReleasedEvent));
        dispatcher.Dispatch<KeyTypedEvent>(FUZE_BIND_EVENT_FN(ImGuiLayer::OnKeyTypedEvent));
        dispatcher.Dispatch<WindowResizedEvent>(FUZE_BIND_EVENT_FN(ImGuiLayer::OnWindowResizedEvent));

        ImGuiIO& io = ImGui::GetIO();

        if (event.IsInCategory(EventCategoryMouse) && io.WantCaptureMouse) {
            event.handled = true;
            return;
        }

        if (event.IsInCategory(EventCategoryKeyboard) && io.WantCaptureKeyboard) {
            event.handled = true;
            return;
        }
    }

    bool ImGuiLayer::OnMouseButtonPressedEvent(MouseButtonEvent& event) {
        ImGuiIO& io = ImGui::GetIO();
        io.AddMouseButtonEvent(event.GetMouseButton(), true);
        return false;
    }

    bool ImGuiLayer::OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& event) {
        ImGuiIO& io = ImGui::GetIO();
        io.AddMouseButtonEvent(event.GetMouseButton(), false);
        return false;
    }

    bool ImGuiLayer::OnMouseMovedEvent(MouseMovedEvent& event) {
        ImGuiIO& io = ImGui::GetIO();
        io.AddMousePosEvent(event.GetX(), event.GetY());
        return false;
    }

    bool ImGuiLayer::OnMouseScrolledEvent(MouseScrolledEvent& event) {
        ImGuiIO& io = ImGui::GetIO();
        io.AddMouseWheelEvent(event.GetXOffset(), event.GetYOffset());
        return false;
    }

    bool ImGuiLayer::OnKeyPressedEvent(KeyPressedEvent& event) {
        ImGuiIO& io = ImGui::GetIO();
        ImGuiKey imguiKey = GLFWKeyToImGuiKey(event.GetKeyCode());
        if (imguiKey != ImGuiKey_None)
            io.AddKeyEvent(imguiKey, true);
        return false;
    }

    bool ImGuiLayer::OnKeyReleasedEvent(KeyReleasedEvent& event) {
        ImGuiIO& io = ImGui::GetIO();
        ImGuiKey imguiKey = GLFWKeyToImGuiKey(event.GetKeyCode());
        if (imguiKey != ImGuiKey_None)
            io.AddKeyEvent(imguiKey, false);
        return false;
    }

    bool ImGuiLayer::OnKeyTypedEvent(KeyTypedEvent& event) {
        ImGuiIO& io = ImGui::GetIO();
        io.AddInputCharacter((unsigned int)event.GetKeyCode());
        return false;
    }

    bool ImGuiLayer::OnWindowResizedEvent(WindowResizedEvent& event) {
        ImGuiIO& io = ImGui::GetIO();
        io.DisplaySize = ImVec2((float)event.GetWidth(), (float)event.GetHeight());
        io.DisplayFramebufferScale = ImVec2(1.f, 1.0f);

        glViewport(0, 0, event.GetWidth(), event.GetHeight());
        return false;
    }

}
