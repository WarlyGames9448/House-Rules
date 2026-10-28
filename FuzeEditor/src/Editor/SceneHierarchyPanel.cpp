#include "Editor/SceneHierarchyPanel.h"

#include <imgui.h>
#include <imgui_internal.h>

#include "Editor/EditorPanel.h"

namespace Fuze {
SceneHierarchyPanel::SceneHierarchyPanel(Ref<Scene> context): m_Context(context) {
}

void SceneHierarchyPanel::SetContext(Ref<Scene> context) {
    m_Context = context;
}

void SceneHierarchyPanel::OnImGuiRenderer() {
    if (UI::BeginPanel("Hierarchy Panel")) {
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
    }

    UI::EndPanel();

    if (UI::BeginPanel("Properties Panel")) {
        if (m_ContextSelection != NULL_ENTITY) {
            DrawEntityProperties(m_ContextSelection);

            if (ImGui::Button("Add Component")) {
                ImGui::OpenPopup("AddComponent");
            }

            // TODO Add Custom component Adding system
            if (ImGui::BeginPopup("AddComponent")) {
                if (ImGui::MenuItem("Camera")) {
                    float aspectRatio = 1280.0f / 720.0f;
                    m_Context->CreateCamera(m_ContextSelection, aspectRatio);
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
    }
    UI::EndPanel();
}

void SceneHierarchyPanel::DrawEntityNode(Entity entity) {
    auto& tag = m_Context->m_Registry->GetComponent<TagComponent>(entity).Tag;

    const bool selected = (m_ContextSelection == entity);

    ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_OpenOnArrow;
    if(selected) flags |= ImGuiTreeNodeFlags_Selected;

    bool opened = ImGui::TreeNodeEx((void*)(uint64_t)entity, flags, "%s", tag.c_str());

    UI::DrawSelectionIndicator(selected);

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
        if (m_Context->GetRegistry()->HasComponent<CameraComponent>(entity)) {
            auto& cameraComp = m_Context->GetRegistry()->GetComponent<CameraComponent>(entity);

            if (cameraComp.Primary) m_Context->SetPrimaryCamera(NULL_ENTITY);
            m_Context->m_CameraSystem->RemoveCameraController(entity);
        }
        m_Context->DestroyEntity(entity);
        if (m_ContextSelection == entity) m_ContextSelection = NULL_ENTITY;
    }
}

template <typename T, typename UIFunction, typename OnRemoveFunction>
void DrawComponent(const std::string& name, Entity entity, Ref<Registry>& registry, UIFunction uiFunction, OnRemoveFunction onRemoveFunction) {

    if (registry->HasComponent<T>(entity)) {
        const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowOverlap;

        bool open = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), treeNodeFlags, "%s", name.c_str());
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
            auto& component = registry->GetComponent<T>(entity);
            uiFunction(component);

            ImGui::TreePop();
        }

        if (removeComponent) {
            onRemoveFunction();
            registry->RemoveComponent<T>(entity);
        }
    }
}

template <typename T, typename UIFunction>
void DrawComponent(const std::string& name, Entity entity, Ref<Registry>& registry, UIFunction uiFunction) {
    DrawComponent<T>(name, entity, registry, uiFunction, []() {});
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
        Ref<Registry> registry = m_Context->m_Registry;
        if (registry->HasComponent<TagComponent>(entity)) {
            auto& tag = registry->GetComponent<TagComponent>(entity).Tag;

            char buffer[256];
            memset(buffer, 0, sizeof(buffer));
            strncpy(buffer, tag.c_str(), sizeof(buffer));
            ImGui::InputText("Tag", buffer, sizeof(buffer));

            tag = buffer;
        }

        DrawComponent<TransformComponent>("Transform", entity, registry, [](auto& component) {
            DrawVec3Control("Translation", component.Translation, 0.0f);
            DrawVec3Control("Rotation", component.Rotation, 0.0f);
            DrawVec3Control("Scale", component.Scale, 1.0f);
        });

        DrawComponent<SpriteRendererComponent>("Sprite", entity, registry, [](auto& component) { ImGui::ColorEdit4("Color", &component.Color.x); });

        DrawComponent<CameraComponent>(
            "Camera",
            entity,
            registry,
            [this, entity](auto& cameraComp) {
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

                bool activateController = cameraComp.ControllerActive;
                ImGui::Checkbox("Activate Controller", &activateController);
                if (activateController != cameraComp.ControllerActive)
                    m_Context->m_CameraSystem->ActivateCameraController(entity, activateController);

                glm::vec3 position = cameraComp.Camera->GetPosition();
                float rotation = cameraComp.Camera->GetRotation();

                DrawVec3Control("Position", position);
                ImGui::DragFloat("Rotation", &rotation);
                cameraComp.Camera->SetPositionAndRotation(position, rotation);

                if (changed) {
                    cameraComp.SizeChanged = true;
                    cameraComp.RecalculateProjection();
                }
            },
            [this, entity]() {
                auto& cameraComp = m_Context->GetRegistry()->GetComponent<CameraComponent>(entity);

                if (cameraComp.Primary) m_Context->SetPrimaryCamera(NULL_ENTITY);
                m_Context->m_CameraSystem->RemoveCameraController(entity);
            });
    }
}
}
