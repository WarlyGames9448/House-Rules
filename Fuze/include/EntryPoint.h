#pragma once
#include "Fuze.h"
#include "Log.h"


#ifdef FUZE_PLATFORM_LINUX

extern Fuze::Application* Fuze::CreateApplication();

int main(int argc, char** argv) {
    Fuze::Log::init();
    Fuze::Log::GetCoreLogger()->warn("Initialized!");
    FUZE_CORE_ERROR("There is an error!");
    FUZE_TRACE("Hello!");
    FUZE_CRITICAL("DIE!");

    auto app = Fuze::CreateApplication();
    app->Run();
    delete app;
    return 0;
}

#endif