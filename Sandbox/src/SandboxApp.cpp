#include "Fuze.h"

class ExampleLayer : public Fuze::Layer {
    public:
        ExampleLayer(): Layer("ExampleLayer") {}

        void OnUpdate() override {

        }
        void OnEvent(Fuze::Event& event) override{

        }

};

class Sandbox : public Fuze::Application
{
public:
    Sandbox()
    {
        PushLayer(new ExampleLayer());
        PushOverlay(new Fuze::ImGuiLayer());
    }

    ~Sandbox()
    {
    }
};

Fuze::Application* Fuze::CreateApplication()
{
    return new Sandbox();
}
