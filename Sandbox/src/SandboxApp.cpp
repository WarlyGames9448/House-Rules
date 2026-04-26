#include "Fuze.h"
#include "EntryPoint.h"

#include "imgui.h"
#include "Sandbox2D.h"

class ColorPickerLayer : public Fuze::Layer {
  public:
    ColorPickerLayer(): Layer("ColorPicker") {
    }

    void OnUpdate(Fuze::Timestep ts) override {
        Fuze::RendererCommand::SetClearColor(m_SquareColor);
        Fuze::RendererCommand::Clear();
    }

    void OnImGuiRender() override {
        ImGui::Begin("ColorPickers");
        ImGui::ColorEdit4("Background", &m_SquareColor.x);
        ImGui::End();
    }

    void OnEvent(Fuze::Event& event) override {
    }

  private:
    glm::vec4 m_SquareColor = {0.0f, 0.0f, 0.0f, 0.0f};
};

class Sandbox : public Fuze::Application {
  public:
    Sandbox() {
        //PushLayer(new ColorPickerLayer());
        PushLayer(new Fuze::Sandbox2D());

        this->Get().GetWindow().SetTitle("Fuze");
    }

    ~Sandbox() {
    }
};

Fuze::Application* Fuze::CreateApplication() {
    return new Sandbox();
}
