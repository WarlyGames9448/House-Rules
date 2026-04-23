#pragma once

#include "Renderer/OrthographicCamera.h"

#include "Events/ApplicationEvent.h"
#include "Events/MouseEvent.h"
#include "Events/Event.h"

namespace Fuze {
class FUZE_API OrthographicCameraController {
  public:
    OrthographicCameraController(float aspectRatio, bool rotationActiveted = false);

    void OnUpdate(float ts);
    void OnEvent(Event& e);

    inline const Ref<OrthographicCamera> GetCamera() const {
        return m_Camera;
    }
    inline Ref<OrthographicCamera> GetCamera() {
        return m_Camera;
    }

    void SetSpeed(float translation, float rotation, float scroll);
    inline void InvertScroll(bool invert) {
        m_InvertScroll = invert;
    }

  private:
    bool OnMouseScrolled(MouseScrolledEvent& e);
    bool OnWindowResized(WindowResizedEvent& e);

  private:
    float m_ZoomLevel = 1.0f;
    float m_AspectRatio;

    float m_TranslationSpeed = 1.0f, m_RotationSpeed = 180.0f, m_ScrollSpeed = 0.5f;

    Ref<OrthographicCamera> m_Camera;

    bool m_RotationActiveted;
    bool m_InvertScroll = false;
};
}
