#include "fuzepch.h"

#include "Renderer/OrthographicCameraController.h"

#include "Events/Event.h"
#include "Input.h"
#include "KeyCodes.h"

namespace Fuze {
OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotationActiveted)
    : m_AspectRatio(aspectRatio), m_RotationActiveted(rotationActiveted) {
    m_Camera =
        CreateRef<OrthographicCamera>(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel, m_NearClip, m_FarClip);
}

OrthographicCameraController::OrthographicCameraController(Ref<OrthographicCamera> camera, float aspectRatio, bool rotationActiveted)
    : m_AspectRatio(aspectRatio), m_Camera(camera), m_RotationActiveted(rotationActiveted) {
}

void OrthographicCameraController::OnUpdate(float ts) {
    FUZE_PROFILE_FUNCTION();

    if (!m_Camera) return;

    glm::vec3 cameraPosition = m_Camera->GetPosition();
    float cameraRotation = m_Camera->GetRotation();

    bool changed = false;

    if (Input::IsKeyPressed(FUZE_KEY_A)) {
        cameraPosition.x -= cos(glm::radians(cameraRotation)) * m_TranslationSpeed * m_ZoomLevel * ts;
        cameraPosition.y -= sin(glm::radians(cameraRotation)) * m_TranslationSpeed * m_ZoomLevel * ts;
        changed = true;
    } else if (Input::IsKeyPressed(FUZE_KEY_D)) {
        cameraPosition.x += cos(glm::radians(cameraRotation)) * m_TranslationSpeed * m_ZoomLevel * ts;
        cameraPosition.y += sin(glm::radians(cameraRotation)) * m_TranslationSpeed * m_ZoomLevel * ts;
        changed = true;
    }

    if (Input::IsKeyPressed(FUZE_KEY_W)) {
        cameraPosition.x += -sin(glm::radians(cameraRotation)) * m_TranslationSpeed * m_ZoomLevel * ts;
        cameraPosition.y += cos(glm::radians(cameraRotation)) * m_TranslationSpeed * m_ZoomLevel * ts;
        changed = true;
    } else if (Input::IsKeyPressed(FUZE_KEY_S)) {
        cameraPosition.x -= -sin(glm::radians(cameraRotation)) * m_TranslationSpeed * m_ZoomLevel * ts;
        cameraPosition.y -= cos(glm::radians(cameraRotation)) * m_TranslationSpeed * m_ZoomLevel * ts;
        changed = true;
    }

    if (changed) m_Camera->SetPosition(cameraPosition);

    if (m_RotationActiveted) {
        if (Input::IsKeyPressed(FUZE_KEY_Q)) {
            cameraRotation += m_RotationSpeed * ts;
            if (cameraRotation > 180.0f) cameraRotation -= 360.0f;
            else if (cameraRotation <= -180.0f) cameraRotation += 360.0f;

            m_Camera->SetRotation(cameraRotation);
        } else if (Input::IsKeyPressed(FUZE_KEY_E)) {
            cameraRotation -= m_RotationSpeed * ts;
            if (cameraRotation > 180.0f) cameraRotation -= 360.0f;
            else if (cameraRotation <= -180.0f) cameraRotation += 360.0f;
            m_Camera->SetRotation(cameraRotation);
        }
    }
}

void OrthographicCameraController::OnEvent(Event& e) {
    FUZE_PROFILE_FUNCTION();

    EventDispatcher dispatcher(e);
    dispatcher.Dispatch<MouseScrolledEvent>(FUZE_BIND_EVENT_FN(OrthographicCameraController::OnMouseScrolled));
    // dispatcher.Dispatch<WindowResizedEvent>(FUZE_BIND_EVENT_FN(OrthographicCameraController::OnWindowResized));
}

void OrthographicCameraController::OnResize(float width, float height) {
    FUZE_PROFILE_FUNCTION();

    m_AspectRatio = width / height;
    CalculateView();
}

bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& e) {
    FUZE_PROFILE_FUNCTION();

    if (m_InvertScroll) m_ZoomLevel += e.GetYOffset() * m_ScrollSpeed;
    else m_ZoomLevel -= e.GetYOffset() * m_ScrollSpeed;

    if (m_ZoomLevel < 0.1) m_ZoomLevel = 0.1;
    CalculateView();
    return true;
}

bool OrthographicCameraController::OnWindowResized(WindowResizedEvent& e) {
    FUZE_PROFILE_FUNCTION();

    OnResize((float)e.GetWidth(), (float)e.GetHeight());
    return true;
}

void OrthographicCameraController::CalculateView() {
    m_Camera->SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel, m_NearClip, m_FarClip);
}

void OrthographicCameraController::SetSpeed(float translation, float rotation, float scroll) {
    FUZE_PROFILE_FUNCTION();

    if (translation > 0) m_TranslationSpeed = translation;
    if (rotation > 0) m_RotationSpeed = rotation;
    if (scroll > 0) m_ScrollSpeed = scroll;
}
}
