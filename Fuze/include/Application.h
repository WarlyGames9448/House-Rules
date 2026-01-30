#pragma once
#include "Core.h"

#include "Events/Event.h"

namespace Fuze {

    class FUZE_API Application {

      public:
        Application();
        virtual ~Application();

        void Run();
    };

    // Definido pelo CLIENTE (Sandbox)
    Application* CreateApplication();
}