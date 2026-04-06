#include "fuzepch.h"

#include "Application.h"
#include "Window.h"

#include "Input.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Renderer/Renderer.h"
#include "Renderer/RendererCommand.h"

//#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "KeyCodes.h"

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

    float vertices[3 * 7] = {
        -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, //
        0.5f,  -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, //
        0.0f,  0.5f,  0.0f, 0.0f, 0.0f, 1.0f, 1.0f, //
    };

    std::shared_ptr<VertexBuffer> vertexBuffer;
    vertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));
    m_VertexArray.reset(VertexArray::Create());

    BufferLayout layout = {
        {ShaderDataType::Float3, "a_Position"},
        {ShaderDataType::Float4,    "a_Color"}
    };

    vertexBuffer->SetLayout(layout);
    m_VertexArray->AddVertexBuffer(vertexBuffer);

    uint32_t indices[3] = {0, 1, 2};
    std::shared_ptr<IndexBuffer> indexBuffer;
    indexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices)));
    m_VertexArray->SetIndexBuffer(indexBuffer);

    const char* vertexShaderSource = R"(
    #version 330 core
    layout (location = 0) in vec3 a_Position;
    layout (location = 1) in vec4 a_Color;

    uniform mat4 projection;

    out vec3 v_Position;
    out vec4 v_Color;
    void main() {
        v_Position = a_Position;
        v_Color = a_Color;
        gl_Position = projection * vec4(a_Position, 1.0);
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



    m_Shader.reset(new Shader(vertexShaderSource, fragmentShaderSource));
    m_projectionLoc = glGetUniformLocation(m_Shader->GetRendererID(), "projection");
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
        RendererCommand::SetClearColor({0.1f, 0.0f, 0.0f, 1});
        RendererCommand::Clear();

        Renderer::BeginScene();
        m_Shader->Bind();
        Renderer::Submit(m_VertexArray);
        // Render Logic --------------------------
        glm::mat4 m_trans;
        m_trans = glm::mat4(1.0f);
        float time = (float)glfwGetTime();
        m_trans = glm::rotate(m_trans, glm::radians(time * 15), glm::vec3(0.0, 0.0, 1.0));


        if(Input::IsMouseButtonPressed(1)){
            m_ortho->MoveX(0.01f);
        }
        if(Input::IsMouseButtonPressed(0)){
            m_ortho->MoveX(-0.01f);
        }
        glm::mat4 projection = m_ortho->GetViewProjection();

        glUniformMatrix4fv(m_projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

        // ----------------------------------------
        Renderer::EndScene();

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
