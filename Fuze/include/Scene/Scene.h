#pragma once

#include "Scene/Registry.h"

#include "Core/Timestep.h"

#include "Renderer/OrthographicCameraController.h"
#include "Scene/CameraSystem.h"

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
    ~Scene();

    Entity CreateEntity(const std::string& tag = "Entity");
    void DestroyEntity(Entity entity);
    Entity CreateCamera(float aspectRatio, const std::string& tag = "Camera", bool hasComponent = false);
    void SetPrimaryCamera(Entity camera);

    // TEMP
    void OnViewportResize(float width, float height) {
        m_CameraSystem->OnViewportResize(width, height);
    }

    void OnViewportResize() {
        m_CameraSystem->OnViewportResize(m_ViewportSize.x, m_ViewportSize.y);
    }

    void OnUpdate(Timestep ts);
    void OnEvent(Event& e) {
        m_CameraSystem->OnEvent(e);
    }

    inline Ref<Registry> GetRegistry() {
        return m_Registry;
    }

  public:
    bool m_ViewportFocused = false;
    glm::vec2 m_ViewportSize;

  private:
    Ref<Registry> m_Registry;
    Ref<RenderSystem> m_RenderSystem;
    Ref<CameraSystem> m_CameraSystem;

    friend class SceneHierarchyPanel;
};
}
