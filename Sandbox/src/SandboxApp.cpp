#include "Application.h"
#include "Events/Event.h"
#include "Fuze.h"

#include "Renderer/RendererAPI.h"
#include "Renderer/RendererCommand.h"
#include "imgui.h"
#include <glm/ext/matrix_transform.hpp>
#include <glm/glm.hpp>

#include <filesystem>

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

// Principal Layer ========================================

class TestLayer : public Fuze::Layer {
  public:
    TestLayer(): Layer("Testing"), m_CameraController(new Fuze::OrthographicCameraController(1280.0f / 720.0f, true)) {
        m_CameraController->SetSpeed(5.0f, 180.0f, 0.05f);
        m_CameraController->InvertScroll(true);
    }

    void OnAttach() override {
        Fuze::RendererCommand::SetBlendMode(Fuze::FUZE_BLEND_ALPHA);

        float vertices[4 * 9] = {
            0.5f,  0.5f,  0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, //
            0.5f,  -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, //
            -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, //
            -0.5f, 0.5f,  0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, //
        };

        Ref<Fuze::VertexBuffer> vertexBuffer = Fuze::VertexBuffer::Create(vertices, sizeof(vertices));

        m_VertexArray = Fuze::VertexArray::Create();

        Fuze::BufferLayout layout = {
            {Fuze::ShaderDataType::Float3, "a_Position"},
            {Fuze::ShaderDataType::Float4,    "a_Color"},
            {Fuze::ShaderDataType::Float2, "a_TexCoord"},
        };

        vertexBuffer->SetLayout(layout);
        m_VertexArray->AddVertexBuffer(vertexBuffer);

        uint32_t indices[6] = {
            0,
            1,
            2,
            2,
            3,
            0,
        };
        Ref<Fuze::IndexBuffer> indexBuffer = Fuze::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));
        m_VertexArray->SetIndexBuffer(indexBuffer);

        std::string current_path = std::string(std::filesystem::current_path());

        m_ShaderLibrary.AddShader("square", current_path + "/Sandbox/assets/shaders/basic.glsl");

        m_Texture1 = Fuze::Texture2D::Create(current_path + "/Sandbox/assets/textures/madruga.jpeg");
        m_Texture2 = Fuze::Texture2D::Create(current_path + "/Sandbox/assets/textures/line.png");
    }

    void OnUpdate(Fuze::Timestep ts) override {
       Fuze::RendererCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1.0f});
        Fuze::RendererCommand::Clear();

        // Input Logic --------------------------
        m_CameraController->OnUpdate(ts);

        // Submits --------------------------------
        Ref<Fuze::Shader> shader = m_ShaderLibrary.GetShader("square");
        Fuze::Renderer::BeginScene(shader, m_CameraController->GetCamera());

        glm::mat4 transform = 1.0f;
        transform = glm::translate(transform, glm::vec3(0.3f, 0.0f, 0.0f));

        m_Texture1->Bind();
        Fuze::Renderer::Submit(shader, m_VertexArray, transform);

        m_Texture2->Bind();
        Fuze::Renderer::Submit(shader, m_VertexArray, transform);

        Fuze::Renderer::EndScene();
    }

    void OnEvent(Fuze::Event& e) override {
        m_CameraController->OnEvent(e);
    }

  private:
    Fuze::ShaderLibrary m_ShaderLibrary;
    Ref<Fuze::VertexArray> m_VertexArray;

    Ref<Fuze::Texture2D> m_Texture1;
    Ref<Fuze::Texture2D> m_Texture2;

    Ref<Fuze::OrthographicCameraController> m_CameraController;
};

class Sandbox : public Fuze::Application {
  public:
    Sandbox() {
        //PushLayer(new ColorPickerLayer());
        PushLayer(new TestLayer());

        this->Get().GetWindow().SetTitle("Fuze");
    }

    ~Sandbox() {
    }
};

Fuze::Application* Fuze::CreateApplication() {
    return new Sandbox();
}
