#include "fuzepch.h"
#include "Window.h"
#include "Application.h"

#include "Input.h"

namespace Fuze {

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

    Application* Application::s_Instance = nullptr;

    Application::Application() {

        FUZE_CORE_ASSERT(!s_Instance, "Application Already exists!");
        s_Instance = this;

        m_Window = std::unique_ptr<Window>(Window::Create());
        m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));
    }

    Application::~Application() {}

    void Application::OnEvent(Event& event) {
        EventDispatcher dispatcher(event);
        dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));
        FUZE_CORE_TRACE("{0}", event);

        for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();) {
            (*--it)->OnEvent(event);
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

            for (Layer* layer : m_LayerStack) {
                layer->OnUpdate();
            }

            auto [x,y] = Input::GetMousePosition();
            FUZE_INFO("{0},{1}", x, y);
        }
    }

    bool Application::OnWindowClose(WindowCloseEvent& event) {
        (void)event;
        m_Running = false;
        return true;
    }

}
