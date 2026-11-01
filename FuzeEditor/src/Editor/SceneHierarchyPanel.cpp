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

    if (UI::BeginPanel("Hierarchy Panel")) {
        DrawHierarchy();
        DrawBackgroundContextMenu();
    }

    UI::EndPanel();

    for (Entity entity : m_PendingDeletion)
        DeleteEntity(entity);

    m_PendingDeletion.clear();
}

void SceneHierarchyPanel::DrawHierarchy() {
    for (auto entity : m_Context->m_Registry->GetEntities())
        DrawEntityNode(entity);

    if (ImGui::IsWindowHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left) && !ImGui::IsAnyItemHovered()) m_SelectionContext.ClearSelection();
}

void SceneHierarchyPanel::DrawBackgroundContextMenu() {
    if (ImGui::BeginPopupContextWindow(nullptr, ImGuiPopupFlags_NoOpenOverItems | ImGuiPopupFlags_MouseButtonRight)) {

        if (ImGui::MenuItem("Create Empty Entity")) CreateEntity();

        ImGui::EndPopup();
    }
}

void SceneHierarchyPanel::DrawEntityNode(Entity entity) {
    auto& tag = m_Context->m_Registry->GetComponent<TagComponent>(entity).Tag;

    const bool selected = (m_SelectionContext.GetSelectedEntity() == entity);

    ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_OpenOnArrow;
    if (selected) flags |= ImGuiTreeNodeFlags_Selected;

    // Keyboard Shortcut F2: Rename
    if (selected && ImGui::IsWindowHovered() && Input::IsKeyPressed(FUZE_KEY_F2)) BeginRename(entity);

    bool opened = false;

    if (m_RenamingEntity == entity) {
        ImGui::SetNextItemWidth(-1.0f);

        if (m_RequestRenameFocus) {
            ImGui::SetKeyboardFocusHere();
            m_RequestRenameFocus = false;
        }

        bool commit = ImGui::InputText("##EntityRename", m_RenameBuffer, sizeof(m_RenameBuffer), ImGuiInputTextFlags_EnterReturnsTrue);

        if (Input::IsKeyPressed(FUZE_KEY_ESCAPE)) {
            CancelRename();
        } else if (commit) {
            tag = m_RenameBuffer;
            m_RenamingEntity = NULL_ENTITY;
        }
    } else {
        opened = ImGui::TreeNodeEx(reinterpret_cast<void*>(static_cast<uintptr_t>(entity)), flags, "%s", tag.c_str());

        if (ImGui::IsItemClicked()) m_SelectionContext.SelectEntity(entity);

        DrawEntityContextMenu(entity);
    }

    UI::DrawSelectionIndicator(selected);

    if (opened) ImGui::TreePop();

    if (selected && ImGui::IsWindowHovered() && Input::IsKeyPressed(FUZE_KEY_DELETE)) {

        m_PendingDeletion.push_back(entity);
    }
}

void SceneHierarchyPanel::CreateEntity() {
    Entity entity = m_Context->CreateEntity("Empty Entity");

    m_SelectionContext.SelectEntity(entity);
}

void SceneHierarchyPanel::DuplicateEntity(Entity entity) {
}

void SceneHierarchyPanel::DeleteEntity(Entity entity) {
    if (!m_Context->m_Registry->HasComponent<TagComponent>(entity)) return;

    if (m_Context->m_Registry->HasComponent<CameraComponent>(entity)) {
        auto& camera = m_Context->m_Registry->GetComponent<CameraComponent>(entity);
        if (camera.Primary) m_Context->SetPrimaryCamera(NULL_ENTITY);
        m_Context->m_CameraSystem->RemoveCameraController(entity);
    }

    if (m_RenamingEntity == entity) CancelRename();

    if (m_SelectionContext.GetSelectedEntity() == entity) m_SelectionContext.ClearSelection();

    m_Context->DestroyEntity(entity);
}

void SceneHierarchyPanel::BeginRename(Entity entity) {
    auto& tag = m_Context->m_Registry->GetComponent<TagComponent>(entity).Tag;

    m_RenamingEntity = entity;

    std::snprintf(m_RenameBuffer, sizeof(m_RenameBuffer), "%s", tag.c_str());

    m_RequestRenameFocus = true;
}

void SceneHierarchyPanel::CancelRename() {
    m_RenamingEntity = NULL_ENTITY;
    m_RequestRenameFocus = false;
}

void SceneHierarchyPanel::DrawEntityContextMenu(Entity entity) {
    if (!ImGui::BeginPopupContextItem()) return;

    if (ImGui::MenuItem("Create Empty Entity")) CreateEntity();

    if (ImGui::MenuItem("Duplicate", "Ctrl+D")) DuplicateEntity(entity);

    if (ImGui::MenuItem("Rename", "F2")) BeginRename(entity);

    ImGui::Separator();

    if (ImGui::MenuItem("Delete", "Delete")) m_PendingDeletion.push_back(entity);

    ImGui::EndPopup();
}

}
