#pragma once
#include "Fuze.h"
#include "ParticleSystem.h"
#include "Scene/Scene.h"

#include "Scene/ScriptableEntity.h"

#include "Editor/SceneHierarchyPanel.h"
#include "Editor/PropertiesPanel.h"
#include "Editor/EditorSelectionContext.h"

#include "Utils/FileUtils.h"

namespace Fuze {

class EditorLayer : public Layer {
  public:
    EditorLayer();

    void OnAttach() override;
    void OnDetach() override;
    void OnUpdate(Timestep ts) override;
    void OnEvent(Event& e) override;
    void OnImGuiRender() override;

  private:
    void NewScene();
    void OpenScene();
    void SaveScene();
    void SaveSceneAs();

    bool OnKeyPressed(KeyPressedEvent& e);

    void SaveOptions();
    void LoadOptions();

  private:
    Ref<Framebuffer> m_Framebuffer;
    bool m_ViewportFocused = false, m_ViewportHovered = false;
    glm::vec2 m_ViewportSize = {0.0f, 0.0f};

    Ref<Scene> m_Scene;
    std::filesystem::path m_ScenePath;
    bool m_IsSceneDirty = false;
    EditorFileContext m_FileContext;

    EditorSelectionContext m_SelectionContext;

    SceneHierarchyPanel m_SceneHierarchyPanel {m_SelectionContext};
    PropertiesPanel m_PropertiesPanel {m_SelectionContext};

    EditorCamera m_EditorCamera;

    friend class Scene;
};
}
