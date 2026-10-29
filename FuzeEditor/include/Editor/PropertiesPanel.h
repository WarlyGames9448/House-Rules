#pragma once

#include "Scene/Scene.h"
#include "EditorPanel.h"
#include "EditorSelectionContext.h"

namespace Fuze {
class PropertiesPanel : EditorPanel {
  public:
    PropertiesPanel(EditorSelectionContext& selectionContext);
    PropertiesPanel(EditorSelectionContext& selectionContext, Ref<Scene> context);

    void OnImGuiRenderer();

    void SetContext(Ref<Scene> context);

    void DrawEntityProperties(Entity entity);

  private:
    Ref<Scene> m_Context;
    EditorSelectionContext& m_SelectionContext;
};
}
