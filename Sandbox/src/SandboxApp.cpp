#include "Fuze.h"

#include "imgui.h"
#include "glm.hpp"

#include <GLFW/glfw3.h>

class ColorPickerLayer : public Fuze::Layer {
  public:
    ColorPickerLayer() : Layer("ColorPicker") {}

    void OnUpdate() override { glClearColor(m_SquareColor.x, m_SquareColor.y, m_SquareColor.z, 1); }

    void OnImGuiRender() override {
        ImGui::Begin("ColorPickers");
        ImGui::ColorEdit3("Background", &m_SquareColor.x);
        ImGui::End();
    }
    void OnEvent(Fuze::Event& event) override {}

  private:
    glm::vec3 m_SquareColor;
};

class Sandbox : public Fuze::Application {
  public:
    Sandbox() { PushLayer(new ColorPickerLayer()); }

    ~Sandbox() {}
};

Fuze::Application* Fuze::CreateApplication() { return new Sandbox(); }
