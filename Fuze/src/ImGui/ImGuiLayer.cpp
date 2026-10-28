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

    ImFont* UIRegular = io.Fonts->AddFontFromFileTTF("Fuze/assets/fonts/Inter-Regular.ttf", 14.0f);

    // Use Inter as the default interface font.
    io.FontDefault = UIRegular;

    ImGui::StyleColorsDark();

    ApplyComponentStyle();
    SetThemeColors();

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

void ImGuiLayer::ApplyComponentStyle() {
    ImGuiStyle& style = ImGui::GetStyle();

    // Geometry
    style.WindowRounding = 2.0f;
    style.ChildRounding = 2.0f;
    style.FrameRounding = 2.0f;
    style.PopupRounding = 2.0f;
    style.ScrollbarRounding = 3.0f;
    style.GrabRounding = 2.0f;
    style.TabRounding = 2.0f;

    // Borders
    style.WindowBorderSize = 1.0f;
    style.ChildBorderSize = 1.0f;
    style.FrameBorderSize = 0.0f;
    style.PopupBorderSize = 1.0f;
    style.TabBorderSize = 0.0f;

    // Spacing
    style.WindowPadding = ImVec2(8.0f, 8.0f);
    style.FramePadding = ImVec2(6.0f, 4.0f);
    style.ItemSpacing = ImVec2(6.0f, 4.0f);
    style.ItemInnerSpacing = ImVec2(4.0f, 4.0f);
    style.IndentSpacing = 14.0f;

    // Controls
    style.ScrollbarSize = 11.0f;
    style.GrabMinSize = 10.0f;
}

void ImGuiLayer::SetThemeColors() {
    // Structural colors
    constexpr ImVec4 Background = {0.098f, 0.110f, 0.114f, 1.0f};    // #191C1D
    constexpr ImVec4 Panel = {0.125f, 0.141f, 0.145f, 1.0f};         // #202425
    constexpr ImVec4 PanelElevated = {0.149f, 0.169f, 0.173f, 1.0f}; // #262B2C
    constexpr ImVec4 Surface = {0.173f, 0.196f, 0.196f, 1.0f};       // #2C3232
    constexpr ImVec4 SurfaceHover = {0.204f, 0.239f, 0.231f, 1.0f};  // #343D3B
    constexpr ImVec4 Border = {0.224f, 0.255f, 0.247f, 1.0f};        // #39413F

    // Interaction colors
    constexpr ImVec4 Accent = {0.329f, 0.486f, 0.439f, 1.0f};        // #547C70
    constexpr ImVec4 AccentHover = {0.392f, 0.561f, 0.506f, 1.0f};   // #648F81
    constexpr ImVec4 AccentPressed = {0.259f, 0.392f, 0.341f, 1.0f}; // #426457
    constexpr ImVec4 AccentSubtle = {0.161f, 0.227f, 0.208f, 1.0f};  // #293A35
    // constexpr ImVec4 AccentBorder = {0.271f, 0.404f, 0.361f, 1.0f};  // #45675C

    // Text colors
    constexpr ImVec4 TextPrimary = {0.835f, 0.855f, 0.843f, 1.0f}; // #D5DAD7
    // constexpr ImVec4 TextSecondary = {0.627f, 0.667f, 0.647f, 1.0f}; // #A0AAA5
    constexpr ImVec4 TextDisabled = {0.408f, 0.443f, 0.427f, 1.0f}; // #68716D
    // constexpr ImVec4 Amber = {0.776f, 0.643f, 0.416f, 1.0f};         // #C6A46A

    ImGuiStyle& style = ImGui::GetStyle();
    ImVec4* colors = style.Colors;

    colors[ImGuiCol_Text] = TextPrimary;
    colors[ImGuiCol_TextDisabled] = TextDisabled;

    colors[ImGuiCol_WindowBg] = Panel;
    colors[ImGuiCol_ChildBg] = Background;
    colors[ImGuiCol_PopupBg] = PanelElevated;

    colors[ImGuiCol_Border] = Border;
    colors[ImGuiCol_BorderShadow] = Background;

    colors[ImGuiCol_FrameBg] = Surface;
    colors[ImGuiCol_FrameBgHovered] = SurfaceHover;
    colors[ImGuiCol_FrameBgActive] = AccentSubtle;

    colors[ImGuiCol_TitleBg] = Panel;
    colors[ImGuiCol_TitleBgActive] = PanelElevated;
    colors[ImGuiCol_TitleBgCollapsed] = Panel;

    colors[ImGuiCol_MenuBarBg] = Panel;
    colors[ImGuiCol_ScrollbarBg] = Background;
    colors[ImGuiCol_ScrollbarGrab] = Border;
    colors[ImGuiCol_ScrollbarGrabHovered] = Accent;
    colors[ImGuiCol_ScrollbarGrabActive] = AccentHover;

    colors[ImGuiCol_CheckMark] = AccentHover;
    colors[ImGuiCol_SliderGrab] = Accent;
    colors[ImGuiCol_SliderGrabActive] = AccentHover;

    colors[ImGuiCol_Button] = AccentSubtle;
    colors[ImGuiCol_ButtonHovered] = Accent;
    colors[ImGuiCol_ButtonActive] = AccentPressed;

    colors[ImGuiCol_Header] = AccentSubtle;
    colors[ImGuiCol_HeaderHovered] = SurfaceHover;
    colors[ImGuiCol_HeaderActive] = AccentPressed;
    colors[ImGuiCol_CheckMark] = Accent;

    colors[ImGuiCol_Separator] = Border;
    colors[ImGuiCol_SeparatorHovered] = Accent;
    colors[ImGuiCol_SeparatorActive] = AccentHover;

    colors[ImGuiCol_ResizeGrip] = AccentSubtle;
    colors[ImGuiCol_ResizeGripHovered] = Accent;
    colors[ImGuiCol_ResizeGripActive] = AccentHover;

    colors[ImGuiCol_Tab] = Panel;
    colors[ImGuiCol_TabHovered] = Accent;
    colors[ImGuiCol_TabSelected] = AccentSubtle;
    colors[ImGuiCol_TabSelectedOverline] = AccentHover;
    colors[ImGuiCol_TabDimmed] = Panel;
    colors[ImGuiCol_TabDimmedSelected] = PanelElevated;
    colors[ImGuiCol_TabDimmedSelectedOverline] = Accent;
}

}
