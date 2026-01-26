#pragma once
#include "Core.hpp"

namespace Fuze {

    class Application
    {
    public:
        Application();
        virtual ~Application();

        void Run();
    };

    // Definido pelo CLIENTE (Sandbox)
    Application* CreateApplication();
}