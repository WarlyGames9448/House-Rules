#pragma once

#include "Application.h"

#ifdef FUZE_PLATFORM_LINUX

extern Fuze::Application* Fuze::CreateApplication();

int main(int argc, char** argv) {
    Fuze::Application* app;
    {
        FUZE_PROFILE_SCOPE("Init");
        Fuze::Log::init();
        app = Fuze::CreateApplication();
    }
    {
        FUZE_PROFILE_SCOPE("Running");

        app->Run();
    }
    {
        FUZE_PROFILE_SCOPE("Delete");
        delete app;
    }

    return 0;
}

#endif
