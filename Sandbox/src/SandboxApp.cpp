#include "Fuze.h"

class Sandbox : public Fuze::Application
{
public:
    Sandbox()
    {
        FUZE_INFO("Log Created");
    }

    ~Sandbox()
    {
    }
};

Fuze::Application* Fuze::CreateApplication()
{
    return new Sandbox();
}