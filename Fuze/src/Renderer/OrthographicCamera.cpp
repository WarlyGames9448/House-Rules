#include "fuzepch.h"

#include "Renderer/OrthographicCamera.h"
#include <glm/gtc/matrix_transform.hpp>

namespace Fuze {
OrthographicCamera::OrthographicCamera() {
    m_Projection = glm::ortho(-1.2f, 1.2f, -1.0f, 1.0f, -1.0f, 1.0f);
    // m_Projection = glm::mat4(1.0f);
    m_View = glm::mat4(1.0f);
    // m_View = glm::rotate(m_View,glm::radians(45.0f), glm::vec3(0.0f, 0.0f, -0.9f));
    RecalculateView();
}

OrthographicCamera::~OrthographicCamera() {}

void OrthographicCamera::RecalculateView() { m_ViewProjection = m_Projection * m_View; }

void OrthographicCamera::MoveX(float xPos) {
    m_View = glm::translate(m_View, glm::vec3(xPos, 0.0f, 0.0f));
    RecalculateView();
}
void OrthographicCamera::MoveY(float yPos) {
    m_View = glm::translate(m_View, glm::vec3(yPos, 0.0f, 0.0f));
    RecalculateView();
}

}
