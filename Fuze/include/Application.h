#pragma once
#include "Core.h"
#include "Events/ApplicationEvent.h"
#include "Window.h"

#include "Events/Event.h"
#include <memory>

namespace Fuze {

    class FUZE_API Application {

      public:
        Application();
        virtual ~Application();

        void OnEvent(Event& event);
        bool OnWindowClose(WindowCloseEvent& event);

        void Run();

      private:
        std::unique_ptr<Window> m_Window;
        bool m_Running = true;
    };

    // Definido pelo CLIENTE (Sandbox)
    Application* CreateApplication();
}