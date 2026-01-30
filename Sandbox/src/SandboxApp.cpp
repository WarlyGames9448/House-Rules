#include "Fuze.h"

class Sandbox : public Fuze::Application
{
public:
    Sandbox()
    {
    }

    ~Sandbox()
    {
    }
};

Fuze::Application* Fuze::CreateApplication()
{
    return new Sandbox();
}