#pragma once

#include "Core.h"

#include "glm/glm.hpp"

namespace Fuze {
class FUZE_API OrthographicCamera {
  public:
    OrthographicCamera();
    OrthographicCamera(float left, float right, float bottom, float top, float zNear = -1.0f, float zFar = 1.0f);
    ~OrthographicCamera();

    void SetPosition(const glm::vec3& position);
    void SetRotation(float degrees);
    void SetPositionAndRotation(const glm::vec3& position, float degrees);
    void SetProjection(float left, float right, float bottom, float top, float zNear = -1.0f, float zFar = 1.0f);

    inline const glm::mat4& GetViewProjection() const {
        return m_ViewProjection;
    }

    const glm::vec3 GetPosition() const {
        return m_Position;
    }

    float GetRotation() const {
        return m_rotation;
    }

  private:
    void RecalculateViewProjection();

  private:
    glm::mat4 m_Projection;
    glm::mat4 m_View;
    glm::mat4 m_ViewProjection;

    glm::vec3 m_Position = {0.0f, 0.0f, 0.0f};
    float m_rotation = 0;
};
}
