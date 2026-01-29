#pragma once
#include "Core.h"

namespace Fuze {

    class FUZE_API Application {

      public:
        Application();
        virtual ~Application();

        void Run();
    };

    // Definido pelo CLIENTE (Sandbox)
    Application *CreateApplication();
} // namespace Fuze