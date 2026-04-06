#include "fuzepch.h"

#include "Renderer/OrthographicCamera.h"
#include <glm/gtc/matrix_transform.hpp>

namespace Fuze {
OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top, float zNear, float zFar)
    : m_Projection(glm::ortho(left, right, bottom, top, zNear, zFar)), m_View(1.0f),
      m_ViewProjection(m_Projection * m_View) {}

OrthographicCamera::~OrthographicCamera() {}

void OrthographicCamera::RecalculateViewProjection() {
    m_View = glm::translate(glm::mat4(1.0f), m_Position) *
             glm::rotate(glm::mat4(1.0f), glm::radians(m_rotation), glm::vec3(0.0f, 0.0f, 1.0f));

    m_View = glm::inverse(m_View);

    m_ViewProjection = m_Projection * m_View;
}

void OrthographicCamera::SetPosition(glm::vec3 position) {
    m_Position += position;
    RecalculateViewProjection();
}
void OrthographicCamera::SetRotation(float rotation) {
    m_rotation += rotation;
    RecalculateViewProjection();
}

}
