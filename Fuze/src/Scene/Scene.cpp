#include "fuzepch.h"

#include "Scene/Scene.h"
#include "Renderer/Renderer2D.h"

namespace Fuze {
// TODO: Make a dispatcher for System call method of other Systems.
// RenderSystem ============================
void RenderSystem::OnUpdate(Timestep ts [[maybe_unused]]) {
    if (!m_Camera) return;

    Renderer2D::BeginScene(m_Camera);
    for (auto& entity : m_Entities) {
        auto transform = m_Registry->GetComponent<TransformComponent>(entity);
        auto sprite = m_Registry->GetComponent<SpriteRendererComponent>(entity);

        Renderer2D::DrawQuad(transform.GetTransform(), sprite.Color);
    }
    Renderer2D::EndScene();
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

void Scene::DestroyEntity(Entity entity) {
    m_Registry->DestroyEntity(entity);
}

Entity Scene::CreateCamera(float aspectRatio, const std::string& tag, bool hasController) {
    auto entity = CreateEntity(tag);
    Ref<OrthographicCamera> camera = CreateRef<OrthographicCamera>(-aspectRatio, aspectRatio, -1.0f, 1.0f, -100.0f, 100.0f);

    m_Registry->AddComponent<CameraComponent>(entity, {camera, hasController});

    Ref<OrthographicCameraController> cameraController = CreateRef<OrthographicCameraController>(camera, aspectRatio, true);

    // TODO
    cameraController->SetSpeed(2.0f, 180.0f, 0.3f);
    cameraController->InvertScroll(true);

    m_CameraSystem->AddCameraController(entity, cameraController);

    return entity;
}

void Scene::CreateCamera(Entity cameraEntity, float aspectRatio, bool hasController) {
    Ref<OrthographicCamera> camera = CreateRef<OrthographicCamera>(-aspectRatio, aspectRatio, -1.0f, 1.0f, -100.0f, 100.0f);

    m_Registry->AddComponent<CameraComponent>(cameraEntity, {camera, hasController});

    Ref<OrthographicCameraController> cameraController = CreateRef<OrthographicCameraController>(camera, aspectRatio, true);

    // TODO
    cameraController->SetSpeed(2.0f, 180.0f, 0.3f);
    cameraController->InvertScroll(true);

    m_CameraSystem->AddCameraController(cameraEntity, cameraController);
}

void Scene::SetPrimaryCamera(Entity camera) {
    m_CameraSystem->SetPrimaryCamera(camera);
    m_RenderSystem->SetCamera(m_CameraSystem->GetPrimaryCamera());
}

void Scene::OnUpdate(Timestep ts) {
    m_CameraSystem->m_UpdateControllers = m_ViewportFocused;
    m_CameraSystem->OnUpdate(ts);
    m_RenderSystem->OnUpdate(ts);
}
}
