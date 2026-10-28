#pragma once

#include "Scene/Scene.h"

namespace Fuze {

class SceneHierarchyPanel {
  public:
    SceneHierarchyPanel() = default;
    SceneHierarchyPanel(Ref<Scene> context);

    void SetContext(Ref<Scene> context);

    void OnImGuiRenderer();

  protected:
    void DrawEntityNode(Entity entity, std::vector<Entity>& pendingDeletion);
    void DrawEntityProperties(Entity entity);

  private:
    Ref<Scene> m_Context;

    Entity m_ContextSelection = NULL_ENTITY;

    Entity m_RenamingEntity = NULL_ENTITY;
    char m_RenameBuffer[256] = {};
    bool m_RequestRenameFocus = false;
};
}
