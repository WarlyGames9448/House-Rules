#pragma once

#include "glm/glm.hpp"

namespace Fuze {
class OrthographicCamera {
  public:
    OrthographicCamera();
    ~OrthographicCamera();

    void MoveX(float xPos);
    void MoveY(float yPos);

    inline glm::mat4 GetViewProjection() { return m_ViewProjection; }

  private:
    void RecalculateView();

  private:
    glm::mat4 m_Projection;
    glm::mat4 m_View;
    glm::mat4 m_ViewProjection;
};
}
