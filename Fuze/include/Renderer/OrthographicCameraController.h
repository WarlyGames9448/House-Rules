#pragma once

#include "Renderer/OrthographicCamera.h"

#include "Events/ApplicationEvent.h"
#include "Events/MouseEvent.h"
#include "Events/Event.h"

namespace Fuze {
class FUZE_API OrthographicCameraController {
  public:
    OrthographicCameraController(float aspectRatio, bool rotationActiveted = false);
    OrthographicCameraController(Ref<OrthographicCamera> camera, float aspectRatio, bool rotationActiveted = false);

    void OnUpdate(float ts);
    void OnEvent(Event& e);
    void OnResize(float width, float height);

    inline const Ref<OrthographicCamera> GetCamera() const {
        return m_Camera;
    }
    inline Ref<OrthographicCamera> GetCamera() {
        return m_Camera;
    }

    void SetSpeed(float translation, float rotation, float scroll);
    inline void RotationActive(bool active) {
        m_RotationActiveted = active;
    }
    inline void InvertScroll(bool invert) {
        m_InvertScroll = invert;
    }

    float GetZoomLevel() const {
        return m_ZoomLevel;
    }
    void SetZoomLevel(float zoomLevel) {
        m_ZoomLevel = zoomLevel;
        CalculateView();
    }

    float GetNearClip() const {
        return m_NearClip;
    }
    void SetNearClip(float nearClip) {
        m_NearClip = nearClip;
        CalculateView();
    }

    float GetFarClip() const {
        return m_FarClip;
    }
    void SetFarClip(float farClip) {
        m_FarClip = farClip;
        CalculateView();
    }

  private:
    void CalculateView();

    bool OnMouseScrolled(MouseScrolledEvent& e);
    bool OnWindowResized(WindowResizedEvent& e);

  private:
    float m_ZoomLevel = 1.0f;
    float m_AspectRatio;

    float m_TranslationSpeed = 1.0f, m_RotationSpeed = 180.0f, m_ScrollSpeed = 0.5f;

    Ref<OrthographicCamera> m_Camera;

    bool m_RotationActiveted;
    bool m_InvertScroll = false;

    float m_NearClip = -1.0f;
    float m_FarClip = 1.0f;
};
}
