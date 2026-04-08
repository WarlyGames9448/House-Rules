#pragma once
#include "Core.h"
#include "Events/ApplicationEvent.h"
#include "Window.h"

#include "Events/Event.h"

#include "Layer.h"
#include "LayerStack.h"
#include "ImGui/ImGuiLayer.h"
#include "Core/Timestep.h"

namespace Fuze {

class FUZE_API Application {

  public:
    Application();
    virtual ~Application();

    void OnEvent(Event& event);

    void PushLayer(Layer* layer);
    void PushOverlay(Layer* layer);

    void Run();

    inline static Application& Get() { return *s_Instance; }
    inline Window& GetWindow() { return *m_Window; }

    Timestep GetDeltaTime() const {return m_DeltaTime;}

  private:
    bool OnWindowClose(WindowCloseEvent& event);

  private:
    std::unique_ptr<Window> m_Window;
    ImGuiLayer* m_ImGuiLayer;
    bool m_Running = true;
    LayerStack m_LayerStack;

    Timestep m_LastFrameTime = 0.0f;
    Timestep m_DeltaTime = 0.0f;

  protected:
    static Application* s_Instance;
};

// Defined by client (Sandbox)
Application* CreateApplication();
}
