#pragma once

#include "Scene/Registry.h"

#include "Core/Timestep.h"

namespace Fuze {
class RenderSystem : public System {
  public:
    void OnUpdate(Timestep ts) override;
};

class Scene {
  public:
    Scene();
    ~Scene();

    void OnUpdate(Timestep ts);

    inline Ref<Registry> GetRegistry() {
        return m_Registry;
    }

  private:
    Ref<Registry> m_Registry;
    Ref<RenderSystem> m_RenderSystem;
};
}
