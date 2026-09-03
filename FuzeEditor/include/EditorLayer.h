#pragma once
#include "Fuze.h"
#include "ParticleSystem.h"
#include "Scene/Scene.h"

#include "Scene/ScriptableEntity.h"

#include "Editor/SceneHierarchyPanel.h"

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
    Ref<Texture2D> m_Texture1;
    Ref<Texture2D> m_Texture2;
    Ref<Texture2D> m_Spritesheet;

    Ref<SubTexture2D> m_floor;

    Entity m_Camera, m_Camera2;
    bool m_ChangeCamera = false;

    Ref<Framebuffer> m_Framebuffer;
    bool m_ViewportFocused = false, m_ViewportHovered = false;
    glm::vec2 m_ViewportSize = {0.0f, 0.0f};

    Entity m_Square, m_Player, testEntity;
    Ref<Scene> m_Scene;
    Ref<NativeScriptSystem> m_NativeScriptSystem;

    SceneHierarchyPanel m_SceneHierarchyPanel;
};
}
