#include "fuzepch.h"

#include "OrthographicCameraController.h"

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
    if (Input::IsKeyPressed(FUZE_KEY_A)) {
        m_Camera->XTranslate(-m_TranslationSpeed * ts * m_ZoomLevel);
    } else if (Input::IsKeyPressed(FUZE_KEY_D)) {
        m_Camera->XTranslate(+m_TranslationSpeed * ts * m_ZoomLevel);
    }

    if (Input::IsKeyPressed(FUZE_KEY_W)) {
        m_Camera->YTranslate(+m_TranslationSpeed * ts * m_ZoomLevel);
    } else if (Input::IsKeyPressed(FUZE_KEY_S)) {
        m_Camera->YTranslate(-m_TranslationSpeed * ts * m_ZoomLevel);
    }

    if (m_RotationActiveted) {
        if (Input::IsKeyPressed(FUZE_KEY_LEFT)) {
            m_Camera->Rotate(m_RotationSpeed * ts);
        } else if (Input::IsKeyPressed(FUZE_KEY_RIGHT)) {
            m_Camera->Rotate(-m_RotationSpeed * ts);
        }
    }
}

void OrthographicCameraController::OnEvent(Event& e) {
    EventDispatcher dispatcher(e);
    dispatcher.Dispatch<MouseScrolledEvent>(FUZE_BIND_EVENT_FN(OrthographicCameraController::OnMouseScrolled));
    dispatcher.Dispatch<WindowResizedEvent>(FUZE_BIND_EVENT_FN(OrthographicCameraController::OnWindowResized));
}

bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& e) {
    if (m_InvertScroll) m_ZoomLevel += e.GetYOffset() * m_ScrollSpeed;
    else m_ZoomLevel -= e.GetYOffset() * m_ScrollSpeed;

    if (m_ZoomLevel < 0.1) m_ZoomLevel = 0.1;
    m_Camera->SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
    return true;
}
bool OrthographicCameraController::OnWindowResized(WindowResizedEvent& e) {
    m_AspectRatio = (float)e.GetWidth() / (float)e.GetHeight();
    m_Camera->SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
    return true;
}

void OrthographicCameraController::SetSpeed(float translation, float rotation, float scroll) {
    if (translation > 0) m_TranslationSpeed = translation;
    if (rotation > 0) m_RotationSpeed = rotation;
    if (scroll > 0) m_ScrollSpeed = scroll;
}
}
