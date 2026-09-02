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
    void DrawEntityNode(Entity entity);
    void DrawEntityProperties(Entity entity);

  private:
    Ref<Scene> m_Context;

    Entity m_ContextSelection = NULL_ENTITY;
};
}
