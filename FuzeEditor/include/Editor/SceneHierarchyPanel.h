#pragma once

#include "Scene/Scene.h"
#include "EditorPanel.h"
#include "EditorSelectionContext.h"

namespace Fuze {

class SceneHierarchyPanel : EditorPanel {
  public:
    SceneHierarchyPanel(EditorSelectionContext& selectionContext);
    SceneHierarchyPanel(EditorSelectionContext& selectionContext, Ref<Scene> context);

    void SetContext(Ref<Scene> context);

    void OnImGuiRenderer();

    // Rendering
    void DrawHierarchy();
    void DrawEntityNode(Entity entity);

    // Context Menus
    void DrawEntityContextMenu(Entity entity);
    void DrawBackgroundContextMenu();
    void DrawToolbar();

    // Entity Operations
    void CreateEntity();
    void DuplicateEntity(Entity entity);
    void DeleteEntity(Entity entity);

    // Rename
    void BeginRename(Entity entity);
    void CancelRename();

    // Input
    void HandleKeyboardNavigation();
  private:
    Ref<Scene> m_Context;
    EditorSelectionContext& m_SelectionContext;

    Entity m_RenamingEntity = NULL_ENTITY;
    char m_RenameBuffer[256] = {};
    bool m_RequestRenameFocus = false;

    std::vector<Entity> m_PendingDeletion;

    bool m_KeyReleased = true;
};
}
