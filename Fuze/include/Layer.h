#pragma once

#include "Core/Timestep.h"
#include "Events/Event.h"

namespace Fuze {
class FUZE_API Layer {
  public:
    Layer(const std::string& name = "Layer");

    virtual ~Layer();
    virtual void OnAttach() {}
    virtual void OnDetach() {}
    virtual void OnUpdate([[maybe_unused]] Timestep ts) {}
    virtual void OnFixedUpdate() {} // For logic and physics
    virtual void OnImGuiRender() {}
    virtual void OnEvent(Event& /*event*/) {}

    inline const std::string GetName() const { return m_DebugName; }

  protected:
    std::string m_DebugName;

  private:
    [[maybe_unused]] bool IsEnabled = true;
};
}
