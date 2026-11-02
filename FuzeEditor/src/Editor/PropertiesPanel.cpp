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

    if (ImGui::Begin("Properties Panel")) {
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
    ImGui::End();
}

template <typename T, typename UIFunction, typename OnRemoveFunction>
void DrawComponent(const std::string& name, Entity entity, Ref<Registry>& registry, UIFunction uiFunction, OnRemoveFunction onRemoveFunction) {

    if (!registry->HasComponent<T>(entity)) return;

    ImGui::PushID(name.c_str());

    const ImGuiTreeNodeFlags treeNodeFlags =
        ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowOverlap | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_Framed;

    bool open = ImGui::TreeNodeEx("Component", treeNodeFlags, "%s", name.c_str());

    ImGui::SameLine(ImGui::GetWindowContentRegionMax().x - ImGui::GetFrameHeight());

    if (ImGui::Button("...", ImVec2(ImGui::GetFrameHeight(), ImGui::GetFrameHeight()))) ImGui::OpenPopup("ComponentSettings");

    bool removeComponent = false;
    if (ImGui::BeginPopup("ComponentSettings")) {
        if (ImGui::MenuItem("Remove component")) removeComponent = true;

        ImGui::EndPopup();
    }

    if (open) {
        ImGui::Indent(4.0f);

        auto& component = registry->GetComponent<T>(entity);
        uiFunction(component);

        ImGui::Unindent(4.0f);

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

template <typename UIFunction> static bool DrawPropertyRow(const char* label, UIFunction&& drawFunction, float labelWidth = 100.0f) {
    bool changed = false;

    ImGui::PushID(label);

    if (ImGui::BeginTable("##PropertyRow", 2, ImGuiTableFlags_SizingStretchProp)) {
        ImGui::TableSetupColumn("##Label", ImGuiTableColumnFlags_WidthFixed, labelWidth);

        ImGui::TableSetupColumn("##Control", ImGuiTableColumnFlags_WidthStretch);

        ImGui::TableNextRow();

        ImGui::TableSetColumnIndex(0);
        ImGui::AlignTextToFramePadding();
        ImGui::TextUnformatted(label);

        ImGui::TableSetColumnIndex(1);
        ImGui::SetNextItemWidth(-1.0f);

        changed = drawFunction();

        ImGui::EndTable();
    }

    ImGui::PopID();

    return changed;
}

static bool DrawVec3Control(const char* label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 100.0f) {
    bool changed = false;

    ImGui::PushID(label);

    if (ImGui::BeginTable("##Vec3Control", 2, ImGuiTableFlags_SizingStretchProp)) {
        ImGui::TableSetupColumn("Label", ImGuiTableColumnFlags_WidthFixed, columnWidth);

        ImGui::TableSetupColumn("Values", ImGuiTableColumnFlags_WidthStretch);

        ImGui::TableNextRow();

        // Label
        ImGui::TableSetColumnIndex(0);

        ImGui::AlignTextToFramePadding();
        ImGui::TextUnformatted(label);

        // Values
        ImGui::TableSetColumnIndex(1);

        const char* axes[] = {"X", "Y", "Z"};

        const float spacing = ImGui::GetStyle().ItemSpacing.x;
        const float availableWidth = ImGui::GetContentRegionAvail().x;

        constexpr float buttonWidth = 22.0f;

        const float groupWidth = (availableWidth - spacing * 2.0f) / 3.0f;
        const float dragWidth = groupWidth - buttonWidth - spacing;

        for (int i = 0; i < 3; i++) {
            ImGui::PushID(i);

            if (ImGui::Button(axes[i], ImVec2(buttonWidth, 0.0f))) {
                values[i] = resetValue;
                changed = true;
            }

            ImGui::SameLine();

            ImGui::SetNextItemWidth(dragWidth);

            changed |= ImGui::DragFloat("##Value", &values[i], 0.1f, 0.0f, 0.0f, "%.2f");

            ImGui::PopID();

            if (i < 2) ImGui::SameLine();
        }

        ImGui::EndTable();
    }

    ImGui::PopID();

    return changed;
}

static bool DrawFloat(const char* label, float& value, float speed = 0.1f, float min = 0.0f, float max = 0.0f) {
    return DrawPropertyRow(label, [&]() { return ImGui::DragFloat("##Value", &value, speed, min, max, "%.2f"); });
}

static bool DrawInt(const char* label, int& value, float speed = 1.0f, int min = 0, int max = 0) {
    return DrawPropertyRow(label, [&]() { return ImGui::DragInt("##Value", &value, speed, min, max); });
}

static bool DrawBool(const char* label, bool& value) {
    return DrawPropertyRow(label, [&]() { return ImGui::Checkbox("##Value", &value); });
}

static bool DrawColor(const char* label, glm::vec4& color) {
    return DrawPropertyRow(label,
                           [&]() { return ImGui::ColorEdit4("##Value", &color.x, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoLabel); });
}

static bool DrawTag(TagComponent& component) {
    char buffer[256];

    std::snprintf(buffer, sizeof(buffer), "%s", component.Tag.c_str());

    bool changed = DrawPropertyRow("Tag", [&]() { return ImGui::InputText("##Value", buffer, sizeof(buffer)); }, 118.0f);

    if (changed) component.Tag = buffer;

    return changed;
}

void PropertiesPanel::DrawEntityProperties(Entity entity) {
    if (entity != NULL_ENTITY) {
        Ref<Registry> registry = m_Context->GetRegistry();
        if (registry->HasComponent<TagComponent>(entity)) {
            auto& tag = registry->GetComponent<TagComponent>(entity);

            DrawTag(tag);
        }

        DrawComponent<TransformComponent>("Transform", entity, registry, [](auto& component) {
            DrawVec3Control("Translation", component.Translation, 0.0f);
            DrawVec3Control("Rotation", component.Rotation, 0.0f);
            DrawVec3Control("Scale", component.Scale, 1.0f);
        });

        DrawComponent<SpriteRendererComponent>("Sprite", entity, registry, [](auto& component) { DrawColor("Color", component.Color); });

        DrawComponent<CameraComponent>(
            "Camera",
            entity,
            registry,
            [this, entity](auto& cameraComp) {
                bool changed = false;

                changed |= DrawFloat("Zoom", cameraComp.Size, 0.1f, 0.1f, 100.0f);
                changed |= DrawFloat("Near Clip", cameraComp.NearClip);
                changed |= DrawFloat("Far Clip", cameraComp.FarClip);

                bool isPrimary = cameraComp.Primary;
                bool changePrimary = isPrimary;
                DrawBool("Set Primary", changePrimary);

                if (changePrimary && !isPrimary) {
                    m_Context->SetPrimaryCamera(entity);
                    if (!cameraComp.FixedAspectRatio) {
                        m_Context->OnViewportResize();
                    }
                } else if (!changePrimary && isPrimary) m_Context->SetPrimaryCamera(NULL_ENTITY);

                bool activateController = cameraComp.ControllerActive;
                DrawBool("Activate Controller", activateController);
                if (activateController != cameraComp.ControllerActive)
                    m_Context->GetCameraSystem()->ActivateCameraController(entity, activateController);

                glm::vec3 position = cameraComp.Camera->GetPosition();
                float rotation = cameraComp.Camera->GetRotation();

                DrawVec3Control("Position", position);
                DrawFloat("Rotation", rotation);
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
