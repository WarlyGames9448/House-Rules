#pragma once

#include "Scene/Registry.h"

#include "Core/Timestep.h"

#include "Renderer/OrthographicCameraController.h"
#include "Scene/CameraSystem.h"

#include "UUID.h"

#include "Renderer/EditorCamera.h"

namespace Fuze {
class RenderSystem : public System {
  public:
    void OnUpdate(Timestep ts) override;

    inline void SetCamera(Ref<OrthographicCamera> camera) {
        m_Camera = camera;
    }

  private:
    Ref<OrthographicCamera> m_Camera;
};

class Scene {
  public:
    Scene();
    Scene(const std::string& name);
    ~Scene();

    void Init();

    Entity CreateEntity(const std::string& tag = "Entity");
    Entity CreateEntityWithUUID(const std::string& id, const std::string& tag = "Entity");
    void DestroyEntity(Entity entity);

    Entity DuplicateEntity(Entity source);
    std::set<Entity> GetAllEntites() const;

    inline int GetEntityCount() const {
        return m_EntityCount;
    }

    Entity CreateCamera(float aspectRatio, const std::string& tag = "Camera");
    void CreateCamera(Entity cameraEntity, float aspectRatio, float zoomLevel = 1.0f);
    void SetPrimaryCamera(Entity camera);

    // TEMP
    void OnViewportResize(float width, float height) {
        m_CameraSystem->OnViewportResize(width, height);
    }

    void OnViewportResize() {
        m_CameraSystem->OnViewportResize(m_ViewportSize.x, m_ViewportSize.y);
    }

    void OnUpdateEditor(Timestep ts, EditorCamera& editorCamera);
    void OnUpdateRuntime(Timestep ts);
    void OnEvent(Event& e) {
        m_CameraSystem->OnEvent(e);
    }

    UUID GetID(Entity entity);

    inline Ref<Registry> GetRegistry() {
        return m_Registry;
    }

    inline Ref<CameraSystem> GetCameraSystem() {
        return m_CameraSystem;
    }

    inline void SetName(const std::string& name) {
        m_Name = name;
    }

    inline const std::string GetName() const {
        return m_Name;
    }

  public:
    bool m_ViewportFocused = false;
    glm::vec2 m_ViewportSize;

  private:
    std::string m_Name;

    Ref<Registry> m_Registry;
    Ref<RenderSystem> m_RenderSystem;
    Ref<CameraSystem> m_CameraSystem;

    int m_EntityCount = 0;

    friend class SceneHierarchyPanel;
    friend class SceneSerializer;
};
}
