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
}
