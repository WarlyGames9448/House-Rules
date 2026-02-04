#pragma once
#include "Core.h"
#include "Events/ApplicationEvent.h"
#include "Window.h"

#include "Events/Event.h"

#include "Layer.h"
#include "LayerStack.h"

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
        bool m_Running = true;
        LayerStack m_LayerStack;

      protected:
        static Application* s_Instance;
    };

    // Definido pelo CLIENTE (Sandbox)
    Application* CreateApplication();
}
