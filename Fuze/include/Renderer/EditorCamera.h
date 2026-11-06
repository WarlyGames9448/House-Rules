#pragma once

#include "Fuze.h"

namespace Fuze {
class EditorCamera {
  public:
    EditorCamera(float aspectRatio, float nearClip, float FarClip);

    void OnUpdate(Timestep ts);
    void OnEvent(Event& e);
    void UpdateProperties();

    inline Ref<OrthographicCamera> GetCamera() const {
        return m_Camera;
    }

    inline const glm::vec2 GetPosition() const {
        return m_Position;
    }

    inline void SetPosition(const glm::vec2& position) {
        m_Position = position;
    }

    inline float GetZoom() const {
        return m_Zoom;
    }

    inline void SetZoom(float zoom) {
        m_Zoom = zoom;
    }

  private:
    void UpdateView();

    void MouseZoom(float delta);
    float ZoomSpeed();

    glm::vec2 ScreenToWorld(const glm::vec2 mousePos);

    bool OnMouseScrolled(MouseScrolledEvent& e);

  private:
    glm::vec2 m_InitialMousePosition = {};
    glm::vec2 m_Position = {};
    glm::vec2 m_CameraStartPosition = {};
    bool m_IsRightButtonPressed;

    float m_Zoom = 1.0f;
    float m_MinZoom = 0.20, m_MaxZoom = 100.0f;

    float m_AspectRatio;
    float m_NearClip = 100.0f;
    float m_FarClip = 100.0f;

    Ref<OrthographicCamera> m_Camera;

    struct Properties {
        glm::vec2 mouseScreenPos = {};
        glm::vec2 windowMin = {};
        glm::vec2 windowPos = {};
        glm::vec2 viewportSize = {};
    };

    Properties m_Prop;
};
}
