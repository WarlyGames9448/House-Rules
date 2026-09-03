#include "Editor/SceneHierarchyPanel.h"

#include <imgui.h>
#include <imgui_internal.h>

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

static void DrawVec3Control(const std::string& label, glm::vec3& values, float resetvalue = 0.0f, float columnWidth = 100.0f) {
    ImGui::PushID(label.c_str());

    ImGui::Columns(2);
    ImGui::SetColumnWidth(0, columnWidth);
    ImGui::Text("%s", label.c_str());
    ImGui::NextColumn();

    ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2 {0.0f, 0.0f});

    float lineWidth = GImGui->FontSize + GImGui->Style.FramePadding.y + 2.0f;
    ImVec2 buttonSize = {lineWidth + 3.0f, lineWidth};

    if (ImGui::Button("X", buttonSize)) values.x = resetvalue;

    ImGui::SameLine();
    ImGui::DragFloat("##X", &values.x, 0.1f);
    ImGui::PopItemWidth();
    ImGui::SameLine();

    if (ImGui::Button("Y", buttonSize)) values.y = resetvalue;

    ImGui::SameLine();
    ImGui::DragFloat("##Y", &values.y, 0.1f);
    ImGui::PopItemWidth();
    ImGui::SameLine();

    if (ImGui::Button("Z", buttonSize)) values.z = resetvalue;

    ImGui::SameLine();
    ImGui::DragFloat("##Z", &values.z, 0.1f);
    ImGui::PopItemWidth();

    ImGui::PopStyleVar();
    ImGui::Columns(1);

    ImGui::PopID();
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
            if (ImGui::TreeNodeEx((void*)typeid(TransformComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Transform")) {
                auto& tc = m_Context->GetRegistry()->GetComponent<TransformComponent>(entity);
                DrawVec3Control("Translation", tc.Translation);
                DrawVec3Control("Rotation", tc.Rotation);
                DrawVec3Control("Scale", tc.Scale);

                ImGui::TreePop();
            }
        }

        if (m_Context->m_Registry->HasComponent<SpriteRendererComponent>(entity)) {
            auto& color = m_Context->GetRegistry()->GetComponent<SpriteRendererComponent>(entity).Color;
            ImGui::ColorEdit4("Color", &color.x);
        }
    }
}
}
