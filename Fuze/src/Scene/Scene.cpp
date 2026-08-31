#include "fuzepch.h"

#include "Scene/Scene.h"
#include "Renderer/Renderer2D.h"

namespace Fuze {
// TODO: Make a dispatcher for System call method of other Systems.
// RenderSystem ============================
void RenderSystem::OnUpdate(Timestep ts [[maybe_unused]]) {
    Renderer2D::BeginScene(m_Camera);
    for (auto& entity : m_Entities) {
        auto transform = m_Registry->GetComponent<TransformComponent>(entity);
        auto sprite = m_Registry->GetComponent<SpriteRendererComponent>(entity);

        Renderer2D::DrawQuad(transform.Transform, sprite.Color);
    }
    Renderer2D::EndScene();
}

// CameraSystem =============================
void CameraSystem::OnUpdate(Timestep ts) {
    if (m_PrimaryCamera != NULL_ENTITY) m_PrimaryCameraController->OnUpdate(ts);
}

void CameraSystem::SetPrimaryCamera(Entity camera) {
    if (m_Entities.contains(camera)) {
        if (m_PrimaryCamera == NULL_ENTITY) {
            m_PrimaryCamera = camera;
        } else {
            auto& oldPrimaryCamera = m_Registry->GetComponent<CameraComponent>(m_PrimaryCamera);
            oldPrimaryCamera.Primary = false;
        }

        auto& newPrimaryCamera = m_Registry->GetComponent<CameraComponent>(camera);
        newPrimaryCamera.Primary = true;
        m_PrimaryCameraController = newPrimaryCamera.CameraController;
    }
}

// Scene ====================================
Scene::Scene() {
    m_Registry = CreateRef<Registry>();

    m_Registry->RegisterComponent<SpriteRendererComponent>();
    m_Registry->RegisterComponent<TransformComponent>();
    m_Registry->RegisterComponent<TagComponent>();
    m_Registry->RegisterComponent<CameraComponent>();

    m_RenderSystem = m_Registry->RegisterSystem<RenderSystem>();

    Signature sceneSignature;
    sceneSignature.set(m_Registry->GetComponentType<SpriteRendererComponent>());
    sceneSignature.set(m_Registry->GetComponentType<TransformComponent>());
    m_Registry->SetSystemSignature<RenderSystem>(sceneSignature);

    m_CameraSystem = m_Registry->RegisterSystem<CameraSystem>();

    Signature cameraSignature;
    cameraSignature.set(m_Registry->GetComponentType<CameraComponent>());
    m_Registry->SetSystemSignature<CameraSystem>(cameraSignature);
}

Scene::~Scene() {
}

Entity Scene::CreateEntity(const std::string& tag) {
    Entity entity = m_Registry->CreateEntity();
    m_Registry->AddComponent<TagComponent>(entity, tag);

    return entity;
}

Entity Scene::CreateCamera(float aspectRatio, const std::string& tag) {
    auto entity = m_Registry->CreateEntity();
    Ref<OrthographicCamera> camera = CreateRef<OrthographicCamera>(-aspectRatio, aspectRatio, -1.0f, 1.0f);
    Ref<OrthographicCameraController> cameraController = CreateRef<OrthographicCameraController>(camera, aspectRatio, true);

    // TODO
    cameraController->SetSpeed(2.0f, 180.0f, 0.1f);
    cameraController->InvertScroll(true);

    m_Registry->AddComponent<TagComponent>(entity, tag);
    m_Registry->AddComponent<CameraComponent>(entity, {camera, cameraController});

    m_CameraSystem->SetPrimaryCamera(entity);
    m_RenderSystem->SetCamera(camera);
    return entity;
}

void Scene::SetPrimaryCamera(Entity camera) {
    m_CameraSystem->SetPrimaryCamera(camera);
    m_RenderSystem->SetCamera(m_CameraSystem->GetPrimaryCamera());
}

void Scene::OnUpdate(Timestep ts) {
    if (m_ViewportFocused) m_CameraSystem->OnUpdate(ts);
    m_RenderSystem->OnUpdate(ts);
}
}
