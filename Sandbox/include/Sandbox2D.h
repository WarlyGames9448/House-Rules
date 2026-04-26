#pragma once
#include "Fuze.h"

namespace Fuze {
class Sandbox2D : public Layer {
  public:
    Sandbox2D();

    void OnAttach() override;
    void OnDetach() override;
    void OnUpdate(Timestep ts) override;
    void OnEvent(Event& e) override;

  private:
    Ref<Texture2D> m_Texture1;
    Ref<Texture2D> m_Texture2;

    Ref<OrthographicCameraController> m_CameraController;
};
}
