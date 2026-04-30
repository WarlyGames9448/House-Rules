#include "fuzepch.h"

#include "Application.h"
#include "Window.h"

#include "Utils/FileUtils.h"

namespace Fuze {

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

Application* Application::s_Instance = nullptr;

Application::Application() {

    FUZE_CORE_ASSERT(!s_Instance, "Application Already exists!");
    s_Instance = this;

    m_Window = Scope<Window>(Window::Create());
    m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));
    m_Window->SetIcon(std::string(FileUtils::GetAppAsset("FuzeLogo.png")));

    m_ImGuiLayer = new ImGuiLayer();
    PushOverlay(m_ImGuiLayer);
}

Application::~Application() {
}

void Application::OnEvent(Event& event) {
    EventDispatcher dispatcher(event);
    dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));
    dispatcher.Dispatch<WindowMinimizedEvent>(BIND_EVENT_FN(OnWindowMinimized));
    dispatcher.Dispatch<WindowRestoredEvent>(BIND_EVENT_FN(OnWindowRestored));

    for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); ++it) {
        (*it)->OnEvent(event);
        if (event.handled) {
            break;
        }
    }
}

void Application::PushLayer(Layer* layer) {
    m_LayerStack.PushLayer(layer);
    layer->OnAttach();
}

void Application::PushOverlay(Layer* layer) {
    m_LayerStack.PushOverlay(layer);
    layer->OnAttach();
}

void Application::Run() {
    while (m_Running) {
        m_Window->OnUpdate();

        m_DeltaTime = m_Window->GetTime() - m_LastFrameTime;
        if (m_DeltaTime > 0.25) m_DeltaTime = 0.25; // Clipping
        m_LastFrameTime = m_Window->GetTime();

        if (!m_Minimized) {
            for (Layer* layer : m_LayerStack) {
                layer->OnUpdate(m_DeltaTime);
            }
        }

        m_ImGuiLayer->Begin();
        for (Layer* layer : m_LayerStack) {
            layer->OnImGuiRender();
        }
        m_ImGuiLayer->End();
    }
}

bool Application::OnWindowClose(WindowCloseEvent& event) {
    (void)event;
    m_Running = false;
    return true;
}

bool Application::OnWindowMinimized(WindowMinimizedEvent& event) {
    (void)event;
    m_Minimized = true;
    return false;
}

bool Application::OnWindowRestored(WindowRestoredEvent& event) {
    (void)event;
    m_Minimized = false;
    return false;
}
}
