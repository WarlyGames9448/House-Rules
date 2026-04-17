#include "Fuze.h"

#include "imgui.h"
#include <glm/ext/matrix_transform.hpp>
#include <glm/glm.hpp>

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

class TestLayer : public Fuze::Layer {
  public:
    TestLayer(): Layer("Testing"), m_ortho(new Fuze::OrthographicCamera(-1.6f, 1.6f, -0.9f, 0.9f, -1.0f, 1.0f)) {
    }

    void OnAttach() override {
        float vertices[4 * 9] = {
            0.5f,  0.5f,  0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, //
            0.5f,  -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, //
            -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, //
            -0.5f, 0.5f,  0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, //
        };

        Ref<Fuze::VertexBuffer> vertexBuffer;
        vertexBuffer.reset(Fuze::VertexBuffer::Create(vertices, sizeof(vertices)));
        m_VertexArray.reset(Fuze::VertexArray::Create());

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
        Ref<Fuze::IndexBuffer> indexBuffer;
        indexBuffer.reset(Fuze::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
        m_VertexArray->SetIndexBuffer(indexBuffer);

        const char* vertexShaderSource = R"(
        #version 330 core
        layout (location = 0) in vec3 a_Position;
        layout (location = 1) in vec4 a_Color;
        layout (location = 2) in vec2 a_TexCoord; // Fixed: vex2 -> vec2

        uniform mat4 u_ViewProjection;
        uniform mat4 u_ModelMatrix;

        out vec3 v_Position;
        out vec4 v_Color;
        out vec2 v_TexCoord;

        void main() {
            v_Position = a_Position;
            v_Color = a_Color;
            v_TexCoord = a_TexCoord;
            gl_Position = u_ViewProjection * u_ModelMatrix * vec4(a_Position, 1.0);
        }
        )";

        const char* fragmentShaderSource = R"(
        #version 330 core
        layout (location = 0) out vec4 color;

        in vec3 v_Position;
        in vec4 v_Color;
        in vec2 v_TexCoord;

        uniform sampler2D u_Texture;

        void main() {
            color = texture(u_Texture, v_TexCoord);
        }
)";

        m_Shader.reset(Fuze::Shader::Create(vertexShaderSource, fragmentShaderSource));
    }

    void OnUpdate(Fuze::Timestep ts) override {
        /* Fuze::RendererCommand::SetClearColor({0.1f, 0.0f, 0.0f, 1});
        Fuze::RendererCommand::Clear(); */

        // FUZE_TRACE("Seconds: {0}, Miliseconds: {1}", ts.GetSeconds(), ts.GetMiliseconds());

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

        // Submits --------------------------------
        Fuze::Renderer::BeginScene(m_Shader, m_ortho);

        for (int i = 0; i < 20; i++) {
            glm::mat4 transform = 1.0f;
            transform = glm::translate(transform, glm::vec3(0.3f * i, 0.2f * i, 0.0f));
            transform = glm::scale(transform, glm::vec3(0.3f, 0.3f, 0.3f * i));
            transform = glm::rotate(transform, glm::radians(30.0f), glm::vec3(0, 0, 1));
            Fuze::Renderer::Submit(m_Shader, m_VertexArray, transform);
        }

        Fuze::Renderer::EndScene();
    }

    void OnEvent(Fuze::Event& event) override {
    }

  private:
    Ref<Fuze::Shader> m_Shader;
    Ref<Fuze::VertexArray> m_VertexArray;

    Fuze::OrthographicCamera* m_ortho;
};

class Sandbox : public Fuze::Application {
  public:
    Sandbox() {
        PushLayer(new ColorPickerLayer());
        PushLayer(new TestLayer());
    }

    ~Sandbox() {
    }
};

Fuze::Application* Fuze::CreateApplication() {
    return new Sandbox();
}
