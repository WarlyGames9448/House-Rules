#pragma once
#include "Core.h"
#include "Events/ApplicationEvent.h"
#include "Window.h"

#include "Events/Event.h"

#include "Layer.h"
#include "LayerStack.h"
#include "ImGui/ImGuiLayer.h"

#include "Renderer/Shader.h"
#include "Renderer/Buffer.h"
#include "Renderer/VertexArray.h"

#include "Renderer/OrthographicCamera.h"


namespace Fuze {

class FUZE_API Application {

  public:
    Application();
    virtual ~Application();

    void OnEvent(Event& event);
    bool OnWindowClose(WindowCloseEvent& event);

    void PushLayer(Layer* layer);
    void PushOverlay(Layer* layer);

    void Run();

    inline static Application& Get() { return *s_Instance; }
    inline Window& GetWindow() { return *m_Window; }

  private:
    std::unique_ptr<Window> m_Window;
    ImGuiLayer* m_ImGuiLayer;
    bool m_Running = true;
    LayerStack m_LayerStack;

    std::shared_ptr<Shader> m_Shader;
    std::shared_ptr<VertexArray> m_VertexArray;

    int m_projectionLoc;
    OrthographicCamera* m_ortho = new OrthographicCamera();

  protected:
    static Application* s_Instance;
};

// Defined by client (Sandbox)
Application* CreateApplication();
}
