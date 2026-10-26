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

    if (ImGui::IsWindowHovered() && ImGui::IsMouseDown(0)) {
        m_ContextSelection = NULL_ENTITY;
    }

    // Right Click on blank space
    if (ImGui::BeginPopupContextWindow(0, ImGuiPopupFlags_NoOpenOverItems | ImGuiPopupFlags_MouseButtonRight)) {
        if (ImGui::MenuItem("Create Empty Entity")) m_Context->CreateEntity("Empty Entity");
        ImGui::EndPopup();
    }

    if (ImGui::IsWindowHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left) && !ImGui::IsAnyItemHovered()) {
    }

    ImGui::End();

    ImGui::Begin("Properties Panel");
    if (m_ContextSelection != NULL_ENTITY) {
        DrawEntityProperties(m_ContextSelection);

        if (ImGui::Button("Add Component")) {
            ImGui::OpenPopup("AddComponent");
        }

        // TODO Add Custom component Adding system
        if (ImGui::BeginPopup("AddComponent")) {
            if (ImGui::MenuItem("Camera")) {
                float aspectRatio = 1280.0f / 720.0f;
                Ref<OrthographicCamera> camera = CreateRef<OrthographicCamera>(-aspectRatio, aspectRatio, -1.0f, 1.0f, -100.0f, 100.0f);
                m_Context->m_Registry->AddComponent<CameraComponent>(m_ContextSelection, {camera, false});
                ImGui::CloseCurrentPopup();
            }

            if (ImGui::MenuItem("SpriteRenderer")) {
                m_Context->m_Registry->AddComponent<SpriteRendererComponent>(m_ContextSelection, {});
                ImGui::CloseCurrentPopup();
            }

            if (ImGui::MenuItem("Transform")) {
                m_Context->m_Registry->AddComponent<TransformComponent>(m_ContextSelection, {});
                ImGui::CloseCurrentPopup();
            }

            ImGui::EndPopup();
        }
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

    bool EntityDeleted = false;
    if (ImGui::BeginPopupContextItem()) {
        if (ImGui::MenuItem("Delete Entity")) EntityDeleted = true;

        ImGui::EndPopup();
    }

    if (opened) {
        ImGui::TreePop();
    }

    if (EntityDeleted) {
        m_Context->DestroyEntity(entity);
        if (m_ContextSelection == entity) m_ContextSelection = NULL_ENTITY;
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

        const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowOverlap;

        if (m_Context->m_Registry->HasComponent<TransformComponent>(entity)) {
            bool open = ImGui::TreeNodeEx((void*)typeid(TransformComponent).hash_code(), treeNodeFlags, "Transform");
            ImGui::SameLine();
            if (ImGui::Button("+")) {
                ImGui::OpenPopup("ComponentSettings");
            }

            bool removeComponent = false;
            if (ImGui::BeginPopup("ComponentSettings")) {
                if (ImGui::MenuItem("Remove component")) removeComponent = true;

                ImGui::EndPopup();
            }

            if (open) {
                auto& tc = m_Context->GetRegistry()->GetComponent<TransformComponent>(entity);
                DrawVec3Control("Translation", tc.Translation, 0.0f);
                DrawVec3Control("Rotation", tc.Rotation, 0.0f);
                DrawVec3Control("Scale", tc.Scale, 1.0f);

                ImGui::TreePop();
            }

            if (removeComponent) m_Context->m_Registry->RemoveComponent<TransformComponent>(entity);
        }

        if (m_Context->m_Registry->HasComponent<SpriteRendererComponent>(entity)) {
            bool open = ImGui::TreeNodeEx((void*)typeid(SpriteRendererComponent).hash_code(), treeNodeFlags, "Sprite");
            ImGui::SameLine();
            if (ImGui::Button("+")) {
                ImGui::OpenPopup("ComponentSettings");
            }

            bool removeComponent = false;
            if (ImGui::BeginPopup("ComponentSettings")) {
                if (ImGui::MenuItem("Remove component")) removeComponent = true;

                ImGui::EndPopup();
            }

            if (open) {
                auto& color = m_Context->GetRegistry()->GetComponent<SpriteRendererComponent>(entity).Color;
                ImGui::ColorEdit4("Color", &color.x);

                ImGui::TreePop();
            }

            if (removeComponent) m_Context->m_Registry->RemoveComponent<SpriteRendererComponent>(entity);
        }

        if (m_Context->m_Registry->HasComponent<CameraComponent>(entity)) {
            bool open = ImGui::TreeNodeEx((void*)typeid(CameraComponent).hash_code(), treeNodeFlags, "Camera");
            ImGui::SameLine();
            if (ImGui::Button("+")) {
                ImGui::OpenPopup("ComponentSettings");
            }

            bool removeComponent = false;
            if (ImGui::BeginPopup("ComponentSettings")) {
                if (ImGui::MenuItem("Remove component")) removeComponent = true;

                ImGui::EndPopup();
            }

            if (open) {

                auto& cameraComp = m_Context->GetRegistry()->GetComponent<CameraComponent>(entity);
                bool changed = false;

                changed |= ImGui::DragFloat("Zoom", &cameraComp.Size, 0.1f, 0.1f, 100.0f);
                changed |= ImGui::DragFloat("Near Clip", &cameraComp.NearClip, 0.1f);
                changed |= ImGui::DragFloat("Far Clip", &cameraComp.FarClip, 0.1f);

                bool isPrimary = cameraComp.Primary;
                bool changePrimary = isPrimary;
                ImGui::Checkbox("Set Primary", &changePrimary);

                if (changePrimary && !isPrimary) {
                    m_Context->SetPrimaryCamera(entity);
                    if (!cameraComp.FixedAspectRatio) {
                        m_Context->OnViewportResize();
                    }
                } else if (!changePrimary && isPrimary) m_Context->SetPrimaryCamera(NULL_ENTITY);

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

            if (removeComponent) m_Context->m_Registry->RemoveComponent<CameraComponent>(entity);
        }
    }
}
}
