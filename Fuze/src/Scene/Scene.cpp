#include "fuzepch.h"

#include "Scene/Scene.h"
#include "Renderer/Renderer2D.h"

namespace Fuze {
// TODO: Make a dispatcher for System call method of other Systems.
// RenderSystem ============================
void RenderSystem::OnUpdate(Timestep ts [[maybe_unused]]) {
    if (!m_Camera) return;

    std::vector<Entity> sortedEntities(m_Entities.begin(), m_Entities.end());

    std::sort(sortedEntities.begin(), sortedEntities.end(), [this](Entity a, Entity b) {
        auto& transformA = m_Registry->GetComponent<TransformComponent>(a);
        auto& transformB = m_Registry->GetComponent<TransformComponent>(b);
        return transformA.Translation.z < transformB.Translation.z;
    });

    Renderer2D::BeginScene(m_Camera);
    for (auto& entity : m_Entities) {
        auto transform = m_Registry->GetComponent<TransformComponent>(entity);
        auto sprite = m_Registry->GetComponent<SpriteRendererComponent>(entity);

        Renderer2D::DrawQuad(transform.GetTransform(), sprite.Color);
    }
    Renderer2D::EndScene();
}

// Scene ====================================
Scene::Scene(): m_Name("Untitled Scene") {
}

Scene::Scene(const std::string& name): m_Name(std::move(name)) {
}

Scene::~Scene() {
}

void Scene::Init() {
    m_Registry = CreateRef<Registry>();

    m_Registry->RegisterComponent<TagComponent>();
    m_Registry->RegisterComponent<IDComponent>([](const IDComponent&) { return IDComponent {}; });
    m_Registry->RegisterComponent<SpriteRendererComponent>();
    m_Registry->RegisterComponent<TransformComponent>();
    m_Registry->RegisterComponent<CameraComponent>([](const CameraComponent& source) {
        CameraComponent clone;

        clone.ControllerActive = source.ControllerActive;
        clone.Primary = false;
        clone.FixedAspectRatio = source.FixedAspectRatio;
        clone.SizeChanged = source.SizeChanged;

        clone.AspectRatio = source.AspectRatio;
        clone.Size = source.Size;
        clone.NearClip = source.NearClip;
        clone.FarClip = source.FarClip;

        clone.Camera = CreateRef<OrthographicCamera>(
            -clone.AspectRatio * clone.Size, clone.AspectRatio * clone.Size, -clone.Size, clone.Size, clone.NearClip, clone.FarClip);

        clone.Camera->SetPosition(source.Camera->GetPosition());
        clone.Camera->SetRotation(source.Camera->GetRotation());

        return clone;
    });

    m_RenderSystem = m_Registry->RegisterSystem<RenderSystem>();

    Signature sceneSignature;
    sceneSignature.set(m_Registry->GetComponentType<SpriteRendererComponent>());
    sceneSignature.set(m_Registry->GetComponentType<TransformComponent>());
    m_Registry->SetSystemSignature<RenderSystem>(sceneSignature);

    m_CameraSystem = m_Registry->RegisterSystem<CameraSystem>();

    Signature cameraSignature;
    cameraSignature.set(m_Registry->GetComponentType<CameraComponent>());
    m_Registry->SetSystemSignature<CameraSystem>(cameraSignature);

    // Camera lifecycle
    m_Registry->RegisterComponentLifecycle<CameraComponent>(
        // On Add
        [this](Entity entity) {
            auto& cameraComponent = m_Registry->GetComponent<CameraComponent>(entity);

            Ref<OrthographicCameraController> controller =
                CreateRef<OrthographicCameraController>(cameraComponent.Camera, cameraComponent.AspectRatio, true);

            controller->SetSpeed(2.0f, 180.0f, 0.3f);
            controller->InvertScroll(true);
            controller->SetZoomLevel(cameraComponent.Size);
            controller->SetNearClip(cameraComponent.NearClip);
            controller->SetFarClip(cameraComponent.FarClip);

            m_CameraSystem->AddCameraController(entity, controller);
        },
        // On Remove
        [this](Entity entity) { m_CameraSystem->RemoveCameraController(entity); });
}

Entity Scene::CreateEntity(const std::string& tag) {
    Entity entity = m_Registry->CreateEntity();
    m_Registry->AddComponent<IDComponent>(entity, {});
    m_Registry->AddComponent<TagComponent>(entity, tag);

    m_EntityCount++;

    return entity;
}

Entity Scene::CreateEntityWithUUID(const std::string& id, const std::string& tag) {
    Entity entity = m_Registry->CreateEntity();
    m_Registry->AddComponent<IDComponent>(entity, UUID(id));
    m_Registry->AddComponent<TagComponent>(entity, tag);

    m_EntityCount++;

    return entity;
}

void Scene::DestroyEntity(Entity entity) {
    if (m_Registry->HasComponent<CameraComponent>(entity)) {
        auto& camera = m_Registry->GetComponent<CameraComponent>(entity);

        if (camera.Primary) SetPrimaryCamera(NULL_ENTITY);
    }

    m_Registry->DestroyEntity(entity);
    m_EntityCount--;
}

Entity Scene::DuplicateEntity(Entity source) {
    Entity destination = m_Registry->CreateEntity();
    m_EntityCount++;

    m_Registry->CopyComponents(source, destination);

    return destination;
}

std::set<Entity> Scene::GetAllEntites() const {
    return m_Registry->GetEntities();
}

Entity Scene::CreateCamera(float aspectRatio, const std::string& tag) {
    auto entity = CreateEntity(tag);
    Ref<OrthographicCamera> camera = CreateRef<OrthographicCamera>(-aspectRatio, aspectRatio, -1.0f, 1.0f, -100.0f, 100.0f);

    m_Registry->AddComponent<CameraComponent>(entity, {camera, false});

    m_EntityCount++;

    return entity;
}

void Scene::CreateCamera(Entity cameraEntity, float aspectRatio, float zoomLevel) {
    CameraComponent cameraComponent;
    cameraComponent.Camera = CreateRef<OrthographicCamera>(-aspectRatio, aspectRatio, -1.0f, 1.0f, -100.0f, 100.0f);

    cameraComponent.AspectRatio = aspectRatio;
    cameraComponent.Size = zoomLevel;

    m_Registry->AddComponent<CameraComponent>(cameraEntity, cameraComponent);
}

void Scene::SetPrimaryCamera(Entity camera) {
    m_CameraSystem->SetPrimaryCamera(camera);
    m_RenderSystem->SetCamera(m_CameraSystem->GetPrimaryCamera());
}

void Scene::OnUpdateEditor(Timestep ts, EditorCamera &editorCamera){
    m_RenderSystem->SetCamera(editorCamera.GetCamera());
    m_RenderSystem->OnUpdate(ts);
}

void Scene::OnUpdateRuntime(Timestep ts) {
    m_CameraSystem->m_UpdateControllers = m_ViewportFocused;
    m_CameraSystem->OnUpdate(ts);
    m_RenderSystem->OnUpdate(ts);
}

UUID Scene::GetID(Entity entity) {
    if (GetRegistry()->HasComponent<IDComponent>(entity)) {
        FUZE_CORE_ASSERT(0, "Entity {0} need IDComponent", entity);
    }
    auto& ID = GetRegistry()->GetComponent<IDComponent>(entity).ID;
    return ID;
}
}
