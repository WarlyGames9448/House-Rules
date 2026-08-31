#pragma once

#include "Scene/Registry.h"

#include "Core/Timestep.h"

#include "Renderer/OrthographicCameraController.h"

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

class CameraSystem : public System {
  public:
    void OnUpdate(Timestep ts) override;
    void OnEvent(Event& e) {
        m_PrimaryCameraController->OnEvent(e);
    }

    void SetPrimaryCamera(Entity camera);

    // TEMP
    void ResizeViewport(float x, float y) {
        m_PrimaryCameraController->OnResize(x, y);
    }

    Ref<OrthographicCamera> GetPrimaryCamera() {
        return m_PrimaryCameraController->GetCamera();
    }

  private:
    Entity m_PrimaryCamera = NULL_ENTITY;
    Ref<OrthographicCameraController> m_PrimaryCameraController;
};

class Scene {
  public:
    Scene();
    ~Scene();

    Entity CreateEntity(const std::string& tag = "Entity");
    Entity CreateCamera(float aspectRatio, const std::string& tag = "Camera");
    void SetPrimaryCamera(Entity camera);

    // TEMP
    void ResizeViewport(float x, float y) {
        m_CameraSystem->ResizeViewport(x, y);
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

  private:
    Ref<Registry> m_Registry;
    Ref<RenderSystem> m_RenderSystem;
    Ref<CameraSystem> m_CameraSystem;

    friend class SceneHierarchyPanel;

};
}
