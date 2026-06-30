#pragma once
#include "Fuze.h"
#include "ParticleSystem.h"

namespace Fuze {
class Sandbox2D : public Layer {
  public:
    Sandbox2D();

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

    Ref<ParticleSystem> m_ParticleSystem;

    float m_Time = 0.0f;
};
}
