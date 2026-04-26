#include "Sandbox2D.h"

#include <glm/gtc/matrix_transform.hpp>
#include "Renderer/Renderer2D.h"

namespace Fuze {
Sandbox2D::Sandbox2D()
    : Layer("2DGameEngine"), m_CameraController(new OrthographicCameraController(1280.0f / 720.0f, true)) {
}

void Sandbox2D::OnAttach() {
    Renderer2D::Init();

    m_CameraController->SetSpeed(5.0f, 180.0f, 0.05f);
    m_CameraController->InvertScroll(true);

    std::string current_path = std::string(std::filesystem::current_path());

    m_Texture1 = Texture2D::Create(current_path + "/Sandbox/assets/textures/madruga.jpeg");
    m_Texture2 = Texture2D::Create(current_path + "/Sandbox/assets/textures/line.png");
}

void Sandbox2D::OnDetach() {
    Renderer2D::Shutdown();
}

void Sandbox2D::OnUpdate(Timestep ts) {
    RendererCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1.0f});
    RendererCommand::Clear();

    // Input Logic --------------------------
    m_CameraController->OnUpdate(ts);

    Renderer2D::BeginScene(m_CameraController->GetCamera());

    Renderer2D::DrawQuad({0.0f,0.0f}, {3.0f, 0.5f}, m_Texture1, 30.0f);
    Renderer2D::DrawQuad({-1.0f, 0.0f, 0.0f}, {2.0f, 1.0f}, {1.0f, 0.0f, 0.0f, 1.0f});
    Renderer2D::DrawQuad({0.0f,0.0f}, {10.0f, 10.0f}, m_Texture2, {1.0f, 1.0f, 0.0f, 0.8f}, 45.0f);

    Renderer2D::EndScene();
}

void Sandbox2D::OnEvent(Event& e) {
    m_CameraController->OnEvent(e);
}

}
