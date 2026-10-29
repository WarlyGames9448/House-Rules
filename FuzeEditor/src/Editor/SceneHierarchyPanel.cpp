#include "Editor/SceneHierarchyPanel.h"

#include <imgui.h>
#include <imgui_internal.h>

#include "Editor/EditorPanel.h"
#include "Input.h"
#include "KeyCodes.h"

namespace Fuze {
SceneHierarchyPanel::SceneHierarchyPanel(EditorSelectionContext& selectionContext)
    : EditorPanel("Scene Hierarchy Panel"), m_SelectionContext(selectionContext) {
}

SceneHierarchyPanel::SceneHierarchyPanel(EditorSelectionContext& selectionContext, Ref<Scene> context)
    : EditorPanel("Scene Hierarchy Panel"), m_Context(context), m_SelectionContext(selectionContext) {
}

void SceneHierarchyPanel::SetContext(Ref<Scene> context) {
    m_Context = context;
    m_SelectionContext.ClearSelection();
}

void SceneHierarchyPanel::OnImGuiRenderer() {
    if (!m_Context) {
        FUZE_ASSERT(0, "Context not setted!");
        return;
    }

    std::vector<Entity> pendingDeletion;

    if (UI::BeginPanel("Hierarchy Panel")) {
        for (auto entity : m_Context->m_Registry->GetEntities())
            DrawEntityNode(entity, pendingDeletion);

        for (Entity entity : pendingDeletion) {
            if (!m_Context->m_Registry->HasComponent<TagComponent>(entity)) continue;

            if (m_Context->m_Registry->HasComponent<CameraComponent>(entity)) {
                auto& camera = m_Context->m_Registry->GetComponent<CameraComponent>(entity);
                if (camera.Primary) m_Context->SetPrimaryCamera(NULL_ENTITY);
                m_Context->m_CameraSystem->RemoveCameraController(entity);
            }

            m_Context->DestroyEntity(entity);

            if (m_SelectionContext.GetSelectedEntity() == entity) m_SelectionContext.ClearSelection();
        }

        if (ImGui::IsWindowHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left) && !ImGui::IsAnyItemHovered()) {
            m_SelectionContext.ClearSelection();
        }

        if (ImGui::BeginPopupContextWindow(nullptr, ImGuiPopupFlags_NoOpenOverItems | ImGuiPopupFlags_MouseButtonRight)) {
            if (ImGui::MenuItem("Create Empty Entity")) m_Context->CreateEntity("Empty Entity");

            ImGui::EndPopup();
        }
    }

    UI::EndPanel();
}

void SceneHierarchyPanel::DrawEntityNode(Entity entity, std::vector<Entity>& pendingDeletion) {
    auto& tag = m_Context->m_Registry->GetComponent<TagComponent>(entity).Tag;

    const bool selected = (m_SelectionContext.GetSelectedEntity() == entity);

    ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_OpenOnArrow;
    if (selected) flags |= ImGuiTreeNodeFlags_Selected;

    // Start renaming the selected entity with F2.
    if (selected && Input::IsKeyPressed(FUZE_KEY_F2)) {
        m_RenamingEntity = entity;

        std::snprintf(m_RenameBuffer, sizeof(m_RenameBuffer), "%s", tag.c_str());

        m_RequestRenameFocus = true;
    }

    bool opened = false;

    if (m_RenamingEntity == entity) {
        ImGui::SetNextItemWidth(-1.0f);

        if (m_RequestRenameFocus) {
            ImGui::SetKeyboardFocusHere();
            m_RequestRenameFocus = false;
        }

        bool commit = ImGui::InputText("##EntityRename", m_RenameBuffer, sizeof(m_RenameBuffer), ImGuiInputTextFlags_EnterReturnsTrue);

        if (Input::IsKeyPressed(FUZE_KEY_ESCAPE)) {
            m_RenamingEntity = NULL_ENTITY;
        } else if (commit) {
            tag = m_RenameBuffer;
            m_RenamingEntity = NULL_ENTITY;
        }
    } else {
        opened = ImGui::TreeNodeEx(reinterpret_cast<void*>(static_cast<uintptr_t>(entity)), flags, "%s", tag.c_str());

        if (ImGui::IsItemClicked()) m_SelectionContext.SelectEntity(entity);
    }

    UI::DrawSelectionIndicator(selected);

    bool entityDeleted = false;

    if (ImGui::BeginPopupContextItem()) {
        if (ImGui::MenuItem("Delete Entity")) entityDeleted = true;

        ImGui::EndPopup();
    }

    if (opened) ImGui::TreePop();

    if (entityDeleted) pendingDeletion.push_back(entity);
}
}
