#include "Editor/SceneHierarchyPanel.h"

#include <imgui.h>

namespace Fuze {
SceneHierarchyPanel::SceneHierarchyPanel(Ref<Scene> context): m_Context(context) {
}

void SceneHierarchyPanel::SetContext(Ref<Scene> context) {
    m_Context = context;
}

void SceneHierarchyPanel::OnImGuiRenderer() {
    ImGui::Begin("Hierarchy Panel");

    for (auto entity : m_Context->m_Registry->GetEntities()) {
        DrawEntityNode(entity);
    }

    if (ImGui::IsWindowHovered() && ImGui::IsMouseClicked(0)) {
        m_ContextSelection = NULL_ENTITY;
    }

    ImGui::End();

    ImGui::Begin("Properties Panel");
    DrawEntityProperties(m_ContextSelection);
    ImGui::End();
}

void SceneHierarchyPanel::DrawEntityNode(Entity entity) {
    auto& tag = m_Context->m_Registry->GetComponent<TagComponent>(entity).Tag;

    ImGuiTreeNodeFlags flags = ((m_ContextSelection == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;

    bool opened = ImGui::TreeNodeEx((void*)(uint64_t)entity, flags, "%s", tag.c_str());

    if (ImGui::IsItemClicked()) {
        m_ContextSelection = entity;
    }

    if (opened) {
        ImGui::TreePop();
    }
}

void SceneHierarchyPanel::DrawEntityProperties(Entity entity) {
    if (entity != NULL_ENTITY) {
        if (m_Context->m_Registry->HasComponent<TagComponent>(entity)) {
            auto& tag = m_Context->m_Registry->GetComponent<TagComponent>(entity).Tag;

            char buffer[256];
            memset(buffer, 0, sizeof(buffer));
            strncpy(buffer, tag.c_str(), sizeof(buffer));
            ImGui::InputText("Tag", buffer, sizeof(buffer));

            tag = buffer;
        }

        if (m_Context->m_Registry->HasComponent<TransformComponent>(entity)) {
            auto& transform = m_Context->GetRegistry()->GetComponent<TransformComponent>(entity).Transform;
            ImGui::SliderFloat2("Transform", &transform[3].x, -1, 1);
        }

        if (m_Context->m_Registry->HasComponent<SpriteRendererComponent>(entity)) {
            auto& color = m_Context->GetRegistry()->GetComponent<SpriteRendererComponent>(entity).Color;
            ImGui::ColorEdit4("Color", &color.x);
        }
    }
}
}
