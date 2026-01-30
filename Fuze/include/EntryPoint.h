#pragma once

#include "Application.h"

#ifdef FUZE_PLATFORM_LINUX

extern Fuze::Application* Fuze::CreateApplication();

int main(int argc, char** argv) {
    Fuze::Log::init();

    auto app = Fuze::CreateApplication();
    app->Run();
    delete app;
    return 0;
}

#endif