#include "fuzepch.h"

#include "Scene/CameraSystem.h"

namespace Fuze {
void CameraSystem::OnUpdate(Timestep ts) {
    for (Entity entity : m_Entities) {
        auto& camera = m_Registry->GetComponent<CameraComponent>(entity);
        if (camera.SizeChanged) {
            if (camera.ControllerActive) {
                m_CameraControllers[entity]->SetZoomLevel(camera.Size);
                m_CameraControllers[entity]->SetFarClip(camera.FarClip);
                m_CameraControllers[entity]->SetNearClip(camera.NearClip);
            }
            camera.SizeChanged = false;
        }
    }

    if (m_PrimaryCamera != NULL_ENTITY && m_UpdateControllers) {
        auto& camera = m_Registry->GetComponent<CameraComponent>(m_PrimaryCamera);

        if (camera.ControllerActive) {
            m_CameraControllers[m_PrimaryCamera]->OnUpdate(ts);
            camera.Size = m_CameraControllers[m_PrimaryCamera]->GetZoomLevel();
        }
    }
}

void CameraSystem::AddCameraController(Entity camera, Ref<OrthographicCameraController> cameraController) {
    m_CameraControllers[camera] = cameraController;
}

void CameraSystem::RemoveCameraController(Entity camera) {
    m_CameraControllers.erase(camera);
}

void CameraSystem::ActivateCameraController(Entity camera, bool status) {
    auto& cameraComp = m_Registry->GetComponent<CameraComponent>(camera);
    cameraComp.ControllerActive = status;
}

void CameraSystem::SetPrimaryCamera(Entity camera) {
    if (camera == NULL_ENTITY) {
        if (m_PrimaryCamera != NULL_ENTITY) {
            auto& oldPrimaryCamera = m_Registry->GetComponent<CameraComponent>(m_PrimaryCamera);
            oldPrimaryCamera.Primary = false;
        }

        m_PrimaryCamera = NULL_ENTITY;

        return;
    }

    if (m_Entities.contains(camera)) {
        if (m_PrimaryCamera == NULL_ENTITY) {
            m_PrimaryCamera = camera;
        } else {
            auto& oldPrimaryCamera = m_Registry->GetComponent<CameraComponent>(m_PrimaryCamera);
            oldPrimaryCamera.Primary = false;
        }

        auto& newPrimaryCamera = m_Registry->GetComponent<CameraComponent>(camera);
        newPrimaryCamera.Primary = true;
        m_PrimaryCamera = camera;

    } else {
        FUZE_CORE_WARN("This camera does not exists.");
    }
}

void CameraSystem::OnViewportResize(float width, float height) {
    if (height == 0) return;

    float aspectRatio = width / height;

    if (m_PrimaryCamera != NULL_ENTITY) {
        auto& camera = m_Registry->GetComponent<CameraComponent>(m_PrimaryCamera);
        camera.AspectRatio = aspectRatio;
        camera.RecalculateProjection();
        m_CameraControllers[m_PrimaryCamera]->OnResize(width, height);
    }
}
}
