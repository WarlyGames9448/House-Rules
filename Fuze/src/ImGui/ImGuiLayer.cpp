#include "fuzepch.h"

#include "ImGui/ImGuiLayer.h"

#include "imgui.h"
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include "Application.h"

#include "GLFW/glfw3.h"

namespace Fuze {

ImGuiLayer::ImGuiLayer(): Layer("ImGuiLayer") {
}

ImGuiLayer::~ImGuiLayer() {
}

bool isWayland() {
    // Primary check
    const char* waylandDisplay = std::getenv("WAYLAND_DISPLAY");
    if (waylandDisplay != nullptr && std::string(waylandDisplay) != "") {
        return true;
    }

    // Fallback check
    const char* xdgSessionType = std::getenv("XDG_SESSION_TYPE");
    if (xdgSessionType != nullptr && std::string(xdgSessionType) == "wayland") {
        return true;
    }

    return false;
}

void ImGuiLayer::OnAttach() {
    FUZE_PROFILE_FUNCTION();

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;

    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigViewportsNoAutoMerge = true;
    io.ConfigViewportsNoTaskBarIcon = true;
    // io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    ImGui::StyleColorsDark();

    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowRounding = 0.0f;
    style.Colors[ImGuiCol_WindowBg].w = 1.0f;

    Application& app = Application::Get();
    GLFWwindow* window = static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 410");
}

void ImGuiLayer::OnDetach() {
    FUZE_PROFILE_FUNCTION();

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void ImGuiLayer::Begin() {
    FUZE_PROFILE_FUNCTION();

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void ImGuiLayer::End() {
    FUZE_PROFILE_FUNCTION();

    ImGuiIO& io = ImGui::GetIO();
    Application& app = Application::Get();
    GLFWwindow* window = static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());

    ImGui::Render();

    int display_w, display_h;
    glfwGetFramebufferSize(window, &display_w, &display_h);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        GLFWwindow* backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);
    }
}

void ImGuiLayer::OnImGuiRender() {
}

void ImGuiLayer::OnEvent(Event& event) {
    (void)event;

    if (m_BlockEvents) {
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
}
}
