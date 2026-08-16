#pragma once

#include "./Layer.h"
#include "Events/MouseEvent.h"
#include "Events/KeyEvent.h"
#include "Events/ApplicationEvent.h"

namespace Fuze {
class FUZE_API ImGuiLayer : public Layer {
  public:
    ImGuiLayer();
    ~ImGuiLayer();

    virtual void OnAttach() override;
    virtual void OnDetach() override;
    virtual void OnEvent(Event& event) override;
    virtual void OnImGuiRender() override;

    void Begin();
    void End();

    void BlockEvents(bool block) {
        m_BlockEvents = block;
    }

  private:
    bool m_BlockEvents = false;
};
}
