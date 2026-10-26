#pragma once

#include "Scene/System.h"
#include "Scene/Registry.h"


namespace Fuze {
class CameraSystem : public System {
  public:
    void OnUpdate(Timestep ts) override;
    void OnEvent(Event& e) {
        m_PrimaryCameraController->OnEvent(e);
    }

    void AddCameraController(Entity camera, Ref<OrthographicCameraController> cameraController);
    //void RemoveCameraController(Entity camera);

    void SetPrimaryCamera(Entity camera);

    void OnViewportResize(float width, float height);

    Ref<OrthographicCamera> GetPrimaryCamera() {
        if(m_PrimaryCamera == NULL_ENTITY) return nullptr;
        return m_Registry->GetComponent<CameraComponent>(m_PrimaryCamera).Camera;
    }

  public:
    bool m_UpdateControllers;

  private:
    Entity m_PrimaryCamera = NULL_ENTITY;
    Ref<OrthographicCameraController> m_PrimaryCameraController;

    std::unordered_map<Entity, Ref<OrthographicCameraController>> m_CameraControllers;

    bool m_IsControllerActive = false;
};
}
