#pragma once

#include "glm/glm.hpp"

namespace Fuze {
class OrthographicCamera {
  public:
    OrthographicCamera(float left, float right, float bottom, float top, float zNear = -1.0f, float zFar = 1.0f);
    ~OrthographicCamera();

    void SetPosition(float x, float y);

    void SetRotation(float degrees);
    void SetProjection(float left, float right, float bottom, float top, float zNear = -1.0f, float zFar = 1.0f);

    inline const glm::mat4& GetViewProjection() const {
        return m_ViewProjection;
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
