#include "EditorLayer.h"

#include "Renderer/Renderer2D.h"
#include "Utils/FileUtils.h"
#include "Random.h"

#include "Editor/EditorPanel.h"

#include "Scene/SceneSerializer.h"

namespace Fuze {

EditorLayer::EditorLayer(): Layer("FuzeEditor") {
}

void EditorLayer::OnAttach() {
    FUZE_PROFILE_FUNCTION();
    RendererCommand::InitializeRenderCaps();

    Renderer2D::Init();

    FramebufferSpecification spec;
    spec.Width = 1280;
    spec.Height = 720;
    m_Framebuffer = Framebuffer::Create(spec);

    m_Scene = CreateRef<Scene>("Main");
    m_Scene->Init();

    m_SceneHierarchyPanel.SetContext(m_Scene);
    m_PropertiesPanel.SetContext(m_Scene);

    FUZE_INFO("{0} {1} {2}",
              RendererCommand::GetRenderCaps().GraphicsAPI,
              RendererCommand::GetRenderCaps().Vendor,
              RendererCommand::GetRenderCaps().Renderer);
}

void EditorLayer::OnDetach() {
    Renderer2D::Shutdown();
}

void EditorLayer::OnUpdate(Timestep ts) {

    const FramebufferSpecification& spec = m_Framebuffer->GetSpecification();
    if (m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f && (spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y)) {
        m_Framebuffer->Resize(m_ViewportSize.x, m_ViewportSize.y);

        m_Scene->OnViewportResize(m_ViewportSize.x, m_ViewportSize.y);
    }

    m_Framebuffer->Bind();
    RendererCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1.0f});
    RendererCommand::Clear();

    Renderer2D::ResetStats();

    m_Scene->OnUpdate(ts);

    m_Framebuffer->Unbind();
    RendererCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1.0f});
    RendererCommand::Clear();
}

void EditorLayer::OnEvent(Event& e) {
    m_Scene->OnEvent(e);

    EventDispatcher dispatcher(e);
    dispatcher.Dispatch<KeyPressedEvent>(FUZE_BIND_EVENT_FN(EditorLayer::OnKeyPressed));
}

void EditorLayer::OnImGuiRender() {
    FUZE_PROFILE_FUNCTION();

    // Getting Viewport and root docking
    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);
    ImGui::SetNextWindowViewport(viewport->ID);

    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse |
                                    ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus |
                                    ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_MenuBar;

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

    ImGui::Begin("RootDockSpace", nullptr, window_flags);
    ImGui::PopStyleVar();

    if (ImGui::BeginMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("New Scene", "Ctrl+N")) NewScene();

            if (ImGui::MenuItem("Open Scene...", "Ctrl+O")) OpenScene();

            ImGui::Separator();

            if (ImGui::MenuItem("Save", "Ctrl+S")) SaveScene();

            if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S")) SaveSceneAs();

            ImGui::Separator();

            if (ImGui::MenuItem("Exit")) Fuze::Application::Get().Close();
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }

    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
        ImGuiID dockspace_id = ImGui::GetID("EngineDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_None);
    }
    ImGui::End();

    m_SceneHierarchyPanel.OnImGuiRenderer();
    m_PropertiesPanel.OnImGuiRenderer();

    UI::PanelOptions options;
    options.Flags = ImGuiWindowFlags_MenuBar;

    // Stats scene ===========
    if (Fuze::UI::BeginPanel("Stats", options)) {
        ImGui::Text("Draw Calls: %d", Renderer2D::GetStats().DrawCalls);
        ImGui::Text("Quad Count: %d", Renderer2D::GetStats().QuadCount);
    }

    UI::EndPanel();

    // Viewport scene ===========
    const ImVec2 zeroPadding(0.0f, 0.0f);

    options.Flags = ImGuiWindowFlags_None;
    options.Padding = &zeroPadding;

    if (UI::BeginPanel("Viewport", options)) {
        m_ViewportFocused = ImGui::IsWindowFocused();
        m_ViewportHovered = ImGui::IsWindowHovered();

        m_Scene->m_ViewportFocused = m_ViewportFocused;

        Fuze::Application::Get().GetImGuiLayer()->BlockEvents(!m_ViewportFocused || !m_ViewportHovered);

        ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();

        m_ViewportSize = {viewportPanelSize.x, viewportPanelSize.y};
        m_Scene->m_ViewportSize = m_ViewportSize;

        if (m_IsSceneDirty) {
            m_Scene->OnViewportResize();
            m_IsSceneDirty = false;
        }

        ImGui::Image(m_Framebuffer->GetColorAttachmentID(), ImVec2 {m_ViewportSize.x, m_ViewportSize.y}, ImVec2 {0, 1}, ImVec2 {1, 0});
    }

    UI::EndPanel();
}

void EditorLayer::NewScene() {
    SaveScene();

    m_Scene = CreateRef<Scene>();
    m_Scene->Init();
    m_SceneHierarchyPanel.SetContext(m_Scene);
    m_PropertiesPanel.SetContext(m_Scene);

    m_ScenePath.clear();

    m_IsSceneDirty = true;
}

void EditorLayer::OpenScene() {
    auto filepath = FileUtils::OpenFile("fuze", m_FileContext.GetLastDirectory());

    if (!filepath) return;

    m_Scene = CreateRef<Scene>();
    m_Scene->Init();
    m_SceneHierarchyPanel.SetContext(m_Scene);
    m_PropertiesPanel.SetContext(m_Scene);

    SceneSerializer sceneSerializer {m_Scene};
    sceneSerializer.DeserializeText(filepath->string());
    m_ScenePath = *filepath;

    m_FileContext.SetLastDirectory(filepath->parent_path());

    m_IsSceneDirty = true;
}

void EditorLayer::SaveScene() {
    if (m_ScenePath.empty()) {
        SaveSceneAs();
        return;
    }

    SceneSerializer serializer(m_Scene);

    if (serializer.SerializeText(m_ScenePath.string())) m_IsSceneDirty = false;
}

void EditorLayer::SaveSceneAs() {
    auto filepath = FileUtils::SaveFile("fuze", "Untitled.fuze", m_FileContext.GetLastDirectory());

    if (!filepath) return;

    SceneSerializer serializer(m_Scene);

    if (serializer.SerializeText(filepath->string())) {
        m_ScenePath = *filepath;
        m_IsSceneDirty = false;
    }

    m_FileContext.SetLastDirectory(filepath->parent_path());
}

bool EditorLayer::OnKeyPressed(KeyPressedEvent& e) {

    // Shortcuts
    if (e.GetRepeatCount() > 1) return true;

    bool control = Input::IsKeyPressed(FUZE_KEY_LEFT_CONTROL) || Input::IsKeyPressed(FUZE_KEY_RIGHT_CONTROL);
    bool shift = Input::IsKeyPressed(FUZE_KEY_LEFT_SHIFT) || Input::IsKeyPressed(FUZE_KEY_RIGHT_SHIFT);

    if(control) FUZE_ERROR("CONTROLLLLL");

    switch (e.GetKeyCode()) {
    case FUZE_KEY_S: {
        if (control && shift) SaveSceneAs();
        if (control && !shift) SaveScene();
        break;
    }
    case FUZE_KEY_O: {
        if (control && !shift) OpenScene();
        break;
    }
    case FUZE_KEY_N: {
        if (control && !shift) NewScene();
        break;
    }
    }

    return true;
}

}
