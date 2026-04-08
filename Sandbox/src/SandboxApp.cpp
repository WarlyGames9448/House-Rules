#include "Fuze.h"

#include "Log.h"
#include "imgui.h"
#include <glm/glm.hpp>

class ColorPickerLayer : public Fuze::Layer {
  public:
    ColorPickerLayer(): Layer("ColorPicker") {}

    void OnUpdate(Fuze::Timestep ts) override {
        Fuze::RendererCommand::SetClearColor(m_SquareColor);
        Fuze::RendererCommand::Clear();
    }

    void OnImGuiRender() override {
        ImGui::Begin("ColorPickers");
        ImGui::ColorEdit4("Background", &m_SquareColor.x);
        ImGui::End();
    }
    void OnEvent(Fuze::Event& event) override {}

  private:
    glm::vec4 m_SquareColor = {0.0f, 0.0f, 0.0f, 0.0f};
};

class TestLayer : public Fuze::Layer {
  public:
    TestLayer(): Layer("Testing"), m_ortho(new Fuze::OrthographicCamera(-1.6f, 1.6f, -1.2f, 1.2f, -1.0f, 1.0f)) {}

    void OnAttach() override {
        float vertices[3 * 7] = {
            -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, //
            0.5f,  -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, //
            0.0f,  0.5f,  0.0f, 0.0f, 0.0f, 1.0f, 1.0f, //
        };

        std::shared_ptr<Fuze::VertexBuffer> vertexBuffer;
        vertexBuffer.reset(Fuze::VertexBuffer::Create(vertices, sizeof(vertices)));
        m_VertexArray.reset(Fuze::VertexArray::Create());

        Fuze::BufferLayout layout = {
            {Fuze::ShaderDataType::Float3, "a_Position"},
            {Fuze::ShaderDataType::Float4,    "a_Color"}
        };

        vertexBuffer->SetLayout(layout);
        m_VertexArray->AddVertexBuffer(vertexBuffer);

        uint32_t indices[3] = {0, 1, 2};
        std::shared_ptr<Fuze::IndexBuffer> indexBuffer;
        indexBuffer.reset(Fuze::IndexBuffer::Create(indices, sizeof(indices)));
        m_VertexArray->SetIndexBuffer(indexBuffer);

        const char* vertexShaderSource = R"(
    #version 330 core
    layout (location = 0) in vec3 a_Position;
    layout (location = 1) in vec4 a_Color;

    uniform mat4 u_ViewProjection;

    out vec3 v_Position;
    out vec4 v_Color;
    void main() {
        v_Position = a_Position;
        v_Color = a_Color;
        gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
    }
    )";

        const char* fragmentShaderSource = R"(
    #version 330 core
    layout (location = 0) out vec4 color;

    in vec3 v_Position;
    in vec4 v_Color;

    void main() {
        color = vec4(v_Position * 0.5 + 0.5, 1.0f);
        color = v_Color;
    }
    )";

        m_Shader.reset(new Fuze::Shader(vertexShaderSource, fragmentShaderSource));
    }

    void OnUpdate(Fuze::Timestep ts) override {
        /* Fuze::RendererCommand::SetClearColor({0.1f, 0.0f, 0.0f, 1});
        Fuze::RendererCommand::Clear(); */

        // FUZE_TRACE("Seconds: {0}, Miliseconds: {1}", ts.GetSeconds(), ts.GetMiliseconds());

        Fuze::Renderer::BeginScene(m_Shader, m_ortho);
        Fuze::Renderer::Submit(m_VertexArray);
        // Input Logic --------------------------
        if (Fuze::Input::IsKeyPressed(FUZE_KEY_A)) {
            m_ortho->SetPosition(glm::vec3(1.0f * ts, 0.0f, 0.0f));
        } else if (Fuze::Input::IsKeyPressed(FUZE_KEY_D)) {
            m_ortho->SetPosition(glm::vec3(-1.0f * ts, 0.0f, 0.0f));
        }
        if (Fuze::Input::IsKeyPressed(FUZE_KEY_W)) {
            m_ortho->SetPosition(glm::vec3(0.0f, -1.0f * ts, 0.0f));
        } else if (Fuze::Input::IsKeyPressed(FUZE_KEY_S)) {
            m_ortho->SetPosition(glm::vec3(0.0f, 1.0f * ts, 0.0f));
        }

        if (Fuze::Input::IsKeyPressed(FUZE_KEY_LEFT)) {
            m_ortho->SetRotation(180.0f * ts);
        } else if (Fuze::Input::IsKeyPressed(FUZE_KEY_RIGHT)) {
            m_ortho->SetRotation(-180.0f * ts);
        }

        // ----------------------------------------
        Fuze::Renderer::EndScene();
    }

    void OnEvent(Fuze::Event& event) override {}

  private:
    std::shared_ptr<Fuze::Shader> m_Shader;
    std::shared_ptr<Fuze::VertexArray> m_VertexArray;

    Fuze::OrthographicCamera* m_ortho;
};

class Sandbox : public Fuze::Application {
  public:
    Sandbox() {
        PushLayer(new ColorPickerLayer());
        PushLayer(new TestLayer());
    }

    ~Sandbox() {}
};

Fuze::Application* Fuze::CreateApplication() { return new Sandbox(); }
