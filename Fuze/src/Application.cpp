#include "Application.hpp"
#include <iostream>

namespace Fuze {

    Application::Application()
    {
    }

    Application::~Application()
    {
    }

    void Application::Run()
    {
        std::cout << "Hello World!" << std::endl;
        while (true);
    }
}