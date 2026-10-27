#pragma once

#include "Scene/System.h"
#include "Scene/Registry.h"

namespace Fuze {
class CameraSystem : public System {
  public:
    void OnUpdate(Timestep ts) override;
    void OnEvent(Event& e) {
        if (m_PrimaryCamera != NULL_ENTITY) m_CameraControllers[m_PrimaryCamera]->OnEvent(e);
    }

    void AddCameraController(Entity camera, Ref<OrthographicCameraController> cameraController);
    void RemoveCameraController(Entity camera);

    void ActivateCameraController(Entity camera, bool status);

    void SetPrimaryCamera(Entity camera);

    void OnViewportResize(float width, float height);

    Ref<OrthographicCamera> GetPrimaryCamera() {
        if (m_PrimaryCamera == NULL_ENTITY) return nullptr;
        return m_Registry->GetComponent<CameraComponent>(m_PrimaryCamera).Camera;
    }

  public:
    bool m_UpdateControllers;

  private:
    Entity m_PrimaryCamera = NULL_ENTITY;

    std::unordered_map<Entity, Ref<OrthographicCameraController>> m_CameraControllers;
};
}
