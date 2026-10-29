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

  protected:
    void DrawEntityNode(Entity entity, std::vector<Entity>& pendingDeletion);

  private:
    Ref<Scene> m_Context;
    EditorSelectionContext& m_SelectionContext;

    Entity m_RenamingEntity = NULL_ENTITY;
    char m_RenameBuffer[256] = {};
    bool m_RequestRenameFocus = false;
};
}
