#include "EditorLayer.h"

#include "Renderer/Renderer2D.h"
#include "Utils/FileUtils.h"
#include "Random.h"

namespace Fuze {
EditorLayer::EditorLayer()
    : Layer("2DGameEngine"), m_CameraController(new OrthographicCameraController(1280.0f / 720.0f, true)), m_ParticleSystem(new ParticleSystem()) {
}

void EditorLayer::OnAttach() {
    FUZE_PROFILE_FUNCTION();
    RendererCommand::InitializeRenderCaps();

    Renderer2D::Init();

    m_CameraController->SetSpeed(5.0f, 180.0f, 0.2f);
    m_CameraController->InvertScroll(true);

    FramebufferSpecification spec;
    spec.Width = 1280;
    spec.Height = 720;
    m_Framebuffer = Framebuffer::Create(spec);

    m_Texture1 = Texture2D::Create(FileUtils::GetSandboxAsset("textures/madruga.jpeg"));
    m_Texture2 = Texture2D::Create(FileUtils::GetSandboxAsset("textures/line.png"));
    m_Spritesheet = Texture2D::Create(FileUtils::GetSandboxAsset("spritesheets/IndoorTight.png"));

    m_floor = SubTexture2D::Create(m_Spritesheet, {24, 15}, {16, 16}, {1, 1});

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

        m_CameraController->OnResize(m_ViewportSize.x, m_ViewportSize.y);
    }

    m_Framebuffer->Bind();

    RendererCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1.0f});
    RendererCommand::Clear();

    // Test variables ------------------------
    m_Time += ts;
    int color = ((int)m_Time) % 255;
    // ---------------------------------------

    m_CameraController->OnUpdate(ts);

    Renderer2D::ResetStats();

    Renderer2D::BeginScene(m_CameraController->GetCamera());

    Renderer2D::DrawQuad({0.0f, 2.0f}, {2.0f, 1.0f}, {1.0f, 0.0f, 0.0f, 1.0f});
    Renderer2D::DrawQuad({-1.0f, 2.0f}, {10.0f, 10.0f}, m_Texture2, 1.0f, {1.0f, 1.0f, 0.0f, 0.8f});
    Renderer2D::DrawQuad({-1.0f, 0.0f}, {2.0f, 3.0f}, {1.0f, 0.0f, 1.0f, 0.5f});
    Renderer2D::DrawQuad({3.0f, 1.0f}, {3.0f, 3.0f}, m_Texture1, 20.0f);

    for (int i = 0; i < 100; i++) {
        Renderer2D::DrawQuad({3.0f, 0.0f + i}, {3.0f, 3.0f}, m_Texture1, 20.0f);
    }

    Renderer2D::DrawRotatedQuad({3.0f, 1.0f}, {2.0f, 1.0f}, glm::radians(m_Time * 50.0f), {0.0f, 1.0f, 0.0f, 1.0f});
    Renderer2D::DrawRotatedQuad({5.0f, 1.0f}, {2.0f, 2.0f}, glm::radians(m_Time * 120.0f), m_Texture1, 2.0f);

    Renderer2D::DrawQuad({-5.0f, -5.0f}, {2.0f, 2.0f}, m_floor);

    Renderer2D::EndScene();

    if (Input::IsKeyPressed(FUZE_KEY_SPACE)) {
        m_ParticleSystem->AddParticle({0.0f, 0.0f},
                                      {0.0f, 0.0f},
                                      {Random::GetFloat(-1.0f, 1.0f), Random::GetFloat(-1.0f, 1.0f)},
                                      {1.0f, 1.0f, 0.0f, 1.0f},
                                      Random::GetFloat(2.0f, 5.0f));
    }

    m_ParticleSystem->OnUpdate(ts);
    m_ParticleSystem->OnRender(m_CameraController->GetCamera());

    m_Framebuffer->Unbind();

    RendererCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1.0f});
    RendererCommand::Clear();
}

void EditorLayer::OnEvent(Event& e) {
    m_CameraController->OnEvent(e);
}

void EditorLayer::OnImGuiRender() {
    FUZE_PROFILE_FUNCTION();
    if (1) { // 0 for desactivate Imgui

        // Getting Viewport and root docking
        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);

        ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse |
                                        ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus |
                                        ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoBackground;

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

        ImGui::Begin("RootDockSpace", nullptr, window_flags);
        ImGui::PopStyleVar();

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

        if (ImGui::BeginMenuBar()) {
            if (ImGui::BeginMenu("File")) {
                if (ImGui::MenuItem("Exit")) Fuze::Application::Get().Close();
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }

        ImGui::End();

        // Viewport scene ===========
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("Viewport");

        ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();

        m_ViewportSize = {viewportPanelSize.x, viewportPanelSize.y};

        ImGui::Image(m_Framebuffer->GetColorAttachmentID(), ImVec2 {m_ViewportSize.x, m_ViewportSize.y}, ImVec2 {0, 1}, ImVec2 {1, 0});
        ImGui::End();
        ImGui::PopStyleVar();
    }
}

}
