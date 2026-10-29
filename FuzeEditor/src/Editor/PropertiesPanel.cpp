#include "Editor/PropertiesPanel.h"

namespace Fuze {
PropertiesPanel::PropertiesPanel(EditorSelectionContext& selectionContext): EditorPanel("Properties Panel"), m_SelectionContext(selectionContext) {
}

PropertiesPanel::PropertiesPanel(EditorSelectionContext& selectionContext, Ref<Scene> context)
    : EditorPanel("Properties Panel"), m_SelectionContext(selectionContext), m_Context(context) {
}

void PropertiesPanel::SetContext(Ref<Scene> context) {
    m_Context = context;
}

void PropertiesPanel::OnImGuiRenderer() {
    if (!m_Context) {
        FUZE_ASSERT(0, "Context not setted!");
        return;
    }

    if (UI::BeginPanel("Properties Panel")) {
        if (m_SelectionContext.HasSelection()) {
            DrawEntityProperties(m_SelectionContext.GetSelectedEntity());

            if (ImGui::Button("Add Component")) {
                ImGui::OpenPopup("AddComponent");
            }

            // TODO Add Custom component Adding system
            if (ImGui::BeginPopup("AddComponent")) {
                if (ImGui::MenuItem("Camera")) {
                    float aspectRatio = 1280.0f / 720.0f;
                    m_Context->CreateCamera(m_SelectionContext.GetSelectedEntity(), aspectRatio);
                    ImGui::CloseCurrentPopup();
                }

                if (ImGui::MenuItem("SpriteRenderer")) {
                    m_Context->GetRegistry()->AddComponent<SpriteRendererComponent>(m_SelectionContext.GetSelectedEntity(), {});
                    ImGui::CloseCurrentPopup();
                }

                if (ImGui::MenuItem("Transform")) {
                    m_Context->GetRegistry()->AddComponent<TransformComponent>(m_SelectionContext.GetSelectedEntity(), {});
                    ImGui::CloseCurrentPopup();
                }

                ImGui::EndPopup();
            }
        }
    }
    UI::EndPanel();
}

template <typename T, typename UIFunction, typename OnRemoveFunction>
void DrawComponent(const std::string& name, Entity entity, Ref<Registry>& registry, UIFunction uiFunction, OnRemoveFunction onRemoveFunction) {

    if (!registry->HasComponent<T>(entity)) return;

    ImGui::PushID(name.c_str());

    const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowOverlap;

    bool open = ImGui::TreeNodeEx("Component", treeNodeFlags, "%s", name.c_str());
    ImGui::SameLine();

    if (ImGui::Button("+")) ImGui::OpenPopup("ComponentSettings");

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

    ImGui::PopID();
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

void PropertiesPanel::DrawEntityProperties(Entity entity) {
    if (entity != NULL_ENTITY) {
        Ref<Registry> registry = m_Context->GetRegistry();
        if (registry->HasComponent<TagComponent>(entity)) {
            auto& tag = registry->GetComponent<TagComponent>(entity).Tag;

            char buffer[256] = {};
            std::snprintf(buffer, sizeof(buffer), "%s", tag.c_str());

            if (ImGui::InputText("Tag", buffer, sizeof(buffer))) {
                tag = buffer;
            }
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
                    m_Context->GetCameraSystem()->ActivateCameraController(entity, activateController);

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
                m_Context->GetCameraSystem()->RemoveCameraController(entity);
            });
    }
}
}
