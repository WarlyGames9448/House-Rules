#pragma once
#include "Fuze.h"
#include "ParticleSystem.h"
#include "Scene/Registry.h"

namespace Fuze {
struct Gravity {
    float force;
};

struct RigidBody {
    float velocity, acceleration;
};

class PhysicsSystem : public System {
  public:
    void Update(float dt);
};

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

    Ref<OrthographicCameraController> m_CameraController;

    Ref<Framebuffer> m_Framebuffer;
    bool m_ViewportFocused = false, m_ViewportHovered = false;
    glm::vec2 m_ViewportSize = {0.0f, 0.0f};

    Ref<ParticleSystem> m_ParticleSystem;

    Entity A, B;
    Ref<PhysicsSystem> m_PhysicsSystem;

    Ref<Registry> m_Registry = CreateRef<Registry>();

    float m_Time = 0.0f;
};
}
