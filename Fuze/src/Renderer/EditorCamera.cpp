#include "Renderer/EditorCamera.h"

namespace Fuze {
EditorCamera::EditorCamera(float aspectRatio, float nearClip, float FarClip)
    : m_AspectRatio(aspectRatio), m_NearClip(nearClip), m_FarClip(FarClip),
      m_Camera(CreateRef<OrthographicCamera>(-aspectRatio, aspectRatio, -1.0f, 1.0f, -100.0f, 100.0f)) {
    UpdateView();
}

void EditorCamera::OnUpdate([[maybe_unused]] Timestep ts) {

    if (ImGui::IsWindowHovered() && Input::IsMouseButtonPressed(FUZE_MOUSE_BUTTON_2)) {
        const glm::vec2 mousePosition {Input::GetMouseX(), Input::GetMouseY()};

        if (!m_IsRightButtonPressed) {
            m_InitialMousePosition = mousePosition;
            m_CameraStartPosition = m_Position;
            m_IsRightButtonPressed = true;
        }

        glm::vec2 mouseDelta = mousePosition - m_InitialMousePosition;

        float worldPerPixel = (2.0f * m_Zoom) / m_Prop.viewportSize.y;

        m_Position.x = m_CameraStartPosition.x - mouseDelta.x * worldPerPixel;
        m_Position.y = m_CameraStartPosition.y + mouseDelta.y * worldPerPixel;
    } else {
        m_IsRightButtonPressed = false;
    }

    if (Input::IsMouseButtonReleased(FUZE_MOUSE_BUTTON_2)) m_IsRightButtonPressed = false;

    float posDelta = m_Zoom * 0.01f;

    if (Input::IsKeyPressed(FUZE_KEY_A)) m_Position.x -= posDelta;
    else if (Input::IsKeyPressed(FUZE_KEY_D)) m_Position.x += posDelta;
    if (Input::IsKeyPressed(FUZE_KEY_W)) m_Position.y += posDelta;
    else if (Input::IsKeyPressed(FUZE_KEY_S)) m_Position.y -= posDelta;

    m_AspectRatio = ImGui::GetWindowWidth() / ImGui::GetWindowHeight();

    UpdateView();
}

void EditorCamera::OnEvent(Event& e) {
    EventDispatcher dispatcher(e);
    dispatcher.Dispatch<MouseScrolledEvent>(FUZE_BIND_EVENT_FN(EditorCamera::OnMouseScrolled));
}

void EditorCamera::UpdateProperties() {
    m_Prop.mouseScreenPos = {ImGui::GetCursorScreenPos().x, ImGui::GetCursorScreenPos().y};

    m_Prop.windowMin = {ImGui::GetWindowContentRegionMin().x, ImGui::GetWindowContentRegionMin().y};
    m_Prop.windowPos = {ImGui::GetWindowPos().x, ImGui::GetWindowPos().y};

    m_Prop.viewportSize = {ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y};
}

void EditorCamera::UpdateView() {
    m_Camera->SetProjection(-m_AspectRatio * m_Zoom, m_AspectRatio * m_Zoom, -m_Zoom, m_Zoom, m_NearClip, m_FarClip);

    m_Camera->SetPosition({m_Position, 1.0f});
}

void EditorCamera::MouseZoom(float delta) {
    if (delta == 0.0f) return;

    float zoomFactor = 1.1f;
    if (delta < 0.0f) m_Zoom /= zoomFactor;
    else m_Zoom *= zoomFactor;

    if (m_Zoom < m_MinZoom) m_Zoom = m_MinZoom;
    if (m_Zoom > m_MaxZoom) m_Zoom = m_MaxZoom;
}

float EditorCamera::ZoomSpeed() {
    return m_Zoom * 0.1f;
}

glm::vec2 EditorCamera::ScreenToWorld(const glm::vec2 mousePos) {
    glm::vec2 viewportTopLeft = m_Prop.windowPos + m_Prop.windowMin;
    glm::vec2 mouseLocal = mousePos - viewportTopLeft;

    glm::vec2 centerOffset = m_Prop.viewportSize * 0.5f;
    glm::vec2 normalizedMouse = mouseLocal - centerOffset;

    normalizedMouse.y = -normalizedMouse.y;

    glm::vec2 world = (normalizedMouse / m_Zoom) + m_Position;

    return world;
}

bool EditorCamera::OnMouseScrolled(MouseScrolledEvent& e) {
    float delta = e.GetYOffset() * 0.1f;
    MouseZoom(delta);
    UpdateView();
    return false;
}

}
