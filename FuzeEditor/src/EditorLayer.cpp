#include "EditorLayer.h"

#include "Renderer/Renderer2D.h"
#include "Utils/FileUtils.h"
#include "Random.h"

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

    m_Texture1 = Texture2D::Create(FileUtils::GetSandboxAsset("textures/madruga.jpeg"));
    m_Texture2 = Texture2D::Create(FileUtils::GetSandboxAsset("textures/line.png"));
    m_Spritesheet = Texture2D::Create(FileUtils::GetSandboxAsset("spritesheets/IndoorTight.png"));
    m_floor = SubTexture2D::Create(m_Spritesheet, {24, 15}, {16, 16}, {1, 1});

    m_Scene = CreateRef<Scene>();
    m_Square = m_Scene->CreateEntity("Square");

    m_Scene->GetRegistry()->AddComponent<TransformComponent>(m_Square, {1.0f});
    m_Scene->GetRegistry()->AddComponent<SpriteRendererComponent>(m_Square, glm::vec4 {1.0f, 0.0f, 0.75f, 1.0f});

    m_Camera = m_Scene->CreateCamera(1280.0f / 720.0f);
    m_Camera2 = m_Scene->CreateCamera(1280.0f / 720.0f);

    FUZE_INFO("{0} {1} {2}",
              RendererCommand::GetRenderCaps().GraphicsAPI,
              RendererCommand::GetRenderCaps().Vendor,
              RendererCommand::GetRenderCaps().Renderer);
}

void EditorLayer::OnDetach() {
    Renderer2D::Shutdown();
}

void EditorLayer::OnUpdate(Timestep ts) {
    // TODO: Move this to cameraSystem
    const FramebufferSpecification& spec = m_Framebuffer->GetSpecification();
    if (m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f && (spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y)) {
        m_Framebuffer->Resize(m_ViewportSize.x, m_ViewportSize.y);

        // TODO: Resize when changing camera
        m_Scene->ResizeViewport(m_ViewportSize.x, m_ViewportSize.y);
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

    // Stats scene ===========
    ImGui::Begin("Stats", nullptr, ImGuiWindowFlags_MenuBar);

    ImGui::Text("Draw Calls: %d", Renderer2D::GetStats().DrawCalls);
    ImGui::Text("Quad Count: %d", Renderer2D::GetStats().QuadCount);

    auto& color = m_Scene->GetRegistry()->GetComponent<SpriteRendererComponent>(m_Square).Color;
    ImGui::ColorEdit3("Square Color", &color.x);

    ImGui::Checkbox("Change Camera", &m_ChangeCamera);
    if (m_ChangeCamera) m_Scene->SetPrimaryCamera(m_Camera2);
    else m_Scene->SetPrimaryCamera(m_Camera);

    ImGui::End();

    // Viewport scene ===========
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("Viewport");

    m_ViewportFocused = ImGui::IsWindowFocused();
    m_ViewportHovered = ImGui::IsWindowHovered();

    m_Scene->m_ViewportFocused = m_ViewportFocused;

    Fuze::Application::Get().GetImGuiLayer()->BlockEvents(!m_ViewportFocused || !m_ViewportHovered);

    ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();

    m_ViewportSize = {viewportPanelSize.x, viewportPanelSize.y};

    ImGui::Image(m_Framebuffer->GetColorAttachmentID(), ImVec2 {m_ViewportSize.x, m_ViewportSize.y}, ImVec2 {0, 1}, ImVec2 {1, 0});
    ImGui::End();
    ImGui::PopStyleVar();
}
}
