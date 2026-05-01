#include "fuzepch.h"

#include "Renderer/OrthographicCameraController.h"

#include "Events/Event.h"
#include "Input.h"
#include "KeyCodes.h"

namespace Fuze {
OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotationActiveted)
    : m_AspectRatio(aspectRatio), m_RotationActiveted(rotationActiveted) {
    m_Camera = std::make_shared<OrthographicCamera>(
        -m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
}

void OrthographicCameraController::OnUpdate(float ts) {
    FUZE_PROFILE_FUNCTION();

    if (Input::IsKeyPressed(FUZE_KEY_A)) {
        m_CameraPosition.x -= cos(glm::radians(m_CameraRotation)) * m_TranslationSpeed * m_ZoomLevel * ts;
        m_CameraPosition.y -= sin(glm::radians(m_CameraRotation)) * m_TranslationSpeed * m_ZoomLevel * ts;
        m_Camera->SetPosition(m_CameraPosition.x, m_CameraPosition.y);
    } else if (Input::IsKeyPressed(FUZE_KEY_D)) {
        m_CameraPosition.x += cos(glm::radians(m_CameraRotation)) * m_TranslationSpeed * m_ZoomLevel * ts;
        m_CameraPosition.y += sin(glm::radians(m_CameraRotation)) * m_TranslationSpeed * m_ZoomLevel * ts;
        m_Camera->SetPosition(m_CameraPosition.x, m_CameraPosition.y);
    }

    if (Input::IsKeyPressed(FUZE_KEY_W)) {
        m_CameraPosition.x += -sin(glm::radians(m_CameraRotation)) * m_TranslationSpeed * m_ZoomLevel * ts;
        m_CameraPosition.y += cos(glm::radians(m_CameraRotation)) * m_TranslationSpeed * m_ZoomLevel * ts;
        m_Camera->SetPosition(m_CameraPosition.x, m_CameraPosition.y);
    } else if (Input::IsKeyPressed(FUZE_KEY_S)) {
        m_CameraPosition.x -= -sin(glm::radians(m_CameraRotation)) * m_TranslationSpeed * m_ZoomLevel * ts;
        m_CameraPosition.y -= cos(glm::radians(m_CameraRotation)) * m_TranslationSpeed * m_ZoomLevel * ts;
        m_Camera->SetPosition(m_CameraPosition.x, m_CameraPosition.y);
    }

    if (m_RotationActiveted) {
        if (Input::IsKeyPressed(FUZE_KEY_Q)) {
            m_CameraRotation += m_RotationSpeed * ts;
            if (m_CameraRotation > 180.0f) m_CameraRotation -= 360.0f;
            else if (m_CameraRotation <= -180.0f) m_CameraRotation += 360.0f;

            m_Camera->SetRotation(m_CameraRotation);
        } else if (Input::IsKeyPressed(FUZE_KEY_E)) {
            m_CameraRotation -= m_RotationSpeed * ts;
            if (m_CameraRotation > 180.0f) m_CameraRotation -= 360.0f;
            else if (m_CameraRotation <= -180.0f) m_CameraRotation += 360.0f;
            m_Camera->SetRotation(m_CameraRotation);
        }
    }
}

void OrthographicCameraController::OnEvent(Event& e) {
    FUZE_PROFILE_FUNCTION();

    EventDispatcher dispatcher(e);
    dispatcher.Dispatch<MouseScrolledEvent>(FUZE_BIND_EVENT_FN(OrthographicCameraController::OnMouseScrolled));
    dispatcher.Dispatch<WindowResizedEvent>(FUZE_BIND_EVENT_FN(OrthographicCameraController::OnWindowResized));
}

bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& e) {
    FUZE_PROFILE_FUNCTION();

    if (m_InvertScroll) m_ZoomLevel += e.GetYOffset() * m_ScrollSpeed;
    else m_ZoomLevel -= e.GetYOffset() * m_ScrollSpeed;

    if (m_ZoomLevel < 0.1) m_ZoomLevel = 0.1;
    m_Camera->SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
    return true;
}

bool OrthographicCameraController::OnWindowResized(WindowResizedEvent& e) {
    FUZE_PROFILE_FUNCTION();

    m_AspectRatio = (float)e.GetWidth() / (float)e.GetHeight();
    m_Camera->SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
    return true;
}

void OrthographicCameraController::SetSpeed(float translation, float rotation, float scroll) {
    FUZE_PROFILE_FUNCTION();

    if (translation > 0) m_TranslationSpeed = translation;
    if (rotation > 0) m_RotationSpeed = rotation;
    if (scroll > 0) m_ScrollSpeed = scroll;
}
}
