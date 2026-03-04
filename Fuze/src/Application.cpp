#include "fuzepch.h"
#include "Window.h"
#include "Application.h"

#include "Input.h"
#include "glad/glad.h"

namespace Fuze {

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

    Application* Application::s_Instance = nullptr;

    Application::Application() {

        FUZE_CORE_ASSERT(!s_Instance, "Application Already exists!");
        s_Instance = this;

        m_Window = std::unique_ptr<Window>(Window::Create());
        m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));

        m_ImGuiLayer = new ImGuiLayer();
        PushOverlay(m_ImGuiLayer);

        float vertices[3 * 3] = {
            -0.5f, -0.5f, 0.0f, //
            0.5f,  -0.5f, 0.0f, //
            0.5f,  0.5f,  0.0f, //
        };

        m_VertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));

        // Create Vertex Array Object
        glGenVertexArrays(1, &m_VertexArray);
        glBindVertexArray(m_VertexArray);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

        uint32_t indices[3] = {0, 1, 2};
        m_IndexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices)));

        const char* vertexShaderSource = R"(
            #version 330 core
            layout (location = 0) in vec3 aPos;

            out vec3 vPos;
            void main() {
                vPos = aPos;
                gl_Position = vec4(aPos, 1.0);
            }
        )";

        const char* fragmentShaderSource = R"(
            #version 330 core
            layout (location = 0) out vec4 FragColor;
            in vec3 vPos;

            void main() {
                FragColor = vec4(vPos, 1.0);
            }
        )";

        m_Shader.reset(new Shader(vertexShaderSource, fragmentShaderSource));
    }

    Application::~Application() {}

    void Application::OnEvent(Event& event) {
        EventDispatcher dispatcher(event);
        dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));

        for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();) {
            (*--it)->OnEvent(event);
            if (event.handled) {
                break;
            }
        }
    }

    void Application::PushLayer(Layer* layer) {
        m_LayerStack.PushLayer(layer);
        layer->OnAttach();
    }

    void Application::PushOverlay(Layer* layer) {
        m_LayerStack.PushOverlay(layer);
        layer->OnAttach();
    }

    void Application::Run() {
        while (m_Running) {

            glClearColor(0.2f, 0.2f, 0.2f, 1);
            glClear(GL_COLOR_BUFFER_BIT);

            m_Shader->Bind();
            glBindVertexArray(m_VertexArray);
            glDrawArrays(GL_TRIANGLES, 0, 3); // 3= sizeof indices

            m_Window->OnUpdate();

            for (Layer* layer : m_LayerStack) {
                layer->OnUpdate();
            }

            m_ImGuiLayer->Begin();
            for (Layer* layer : m_LayerStack) {
                layer->OnImGuiRender();
            }
            m_ImGuiLayer->End();

            /* auto [x,y] = Input::GetMousePosition();
            FUZE_INFO("{0},{1}", x, y); */
        }
    }

    bool Application::OnWindowClose(WindowCloseEvent& event) {
        (void)event;
        m_Running = false;
        return true;
    }

}
