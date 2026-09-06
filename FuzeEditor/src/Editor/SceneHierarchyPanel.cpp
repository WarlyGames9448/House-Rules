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
                DrawVec3Control("Translation", tc.Translation, 0.0f);
                DrawVec3Control("Rotation", tc.Rotation, 0.0f);
                DrawVec3Control("Scale", tc.Scale, 1.0f);

                ImGui::TreePop();
            }
        }

        if (m_Context->m_Registry->HasComponent<SpriteRendererComponent>(entity)) {
            if (ImGui::TreeNodeEx((void*)typeid(SpriteRendererComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Sprite")) {
                auto& color = m_Context->GetRegistry()->GetComponent<SpriteRendererComponent>(entity).Color;
                ImGui::ColorEdit4("Color", &color.x);

                ImGui::TreePop();
            }
        }

        if (m_Context->m_Registry->HasComponent<CameraComponent>(entity)) {
            if (ImGui::TreeNodeEx((void*)typeid(CameraComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Camera")) {

                auto& cameraComp = m_Context->GetRegistry()->GetComponent<CameraComponent>(entity);
                bool changed = false;

                changed |= ImGui::DragFloat("Zoom", &cameraComp.Size, 0.1f, 0.1f, 100.0f);
                changed |= ImGui::DragFloat("Near Clip", &cameraComp.NearClip, 0.1f);
                changed |= ImGui::DragFloat("Far Clip", &cameraComp.FarClip, 0.1f);

                glm::vec3 position = cameraComp.Camera->GetPosition();
                float rotation = cameraComp.Camera->GetRotation();

                DrawVec3Control("Position", position);
                ImGui::DragFloat("Rotation", &rotation);
                cameraComp.Camera->SetPositionAndRotation(position, rotation);

                if (changed) {
                    cameraComp.SizeChanged = true;
                    cameraComp.RecalculateProjection();
                }
                ImGui::TreePop();
            }
        }
    }
}
}
