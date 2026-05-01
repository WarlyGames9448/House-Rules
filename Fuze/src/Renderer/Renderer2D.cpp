#include "fuzepch.h"

#include "Renderer/Renderer2D.h"

#include "Renderer/RendererCommand.h"
#include <glm/gtc/matrix_transform.hpp>

#include "Utils/FileUtils.h"

namespace Fuze {

struct Renderer2DStorage {
    Ref<Shader> shader;
    Ref<VertexArray> vertexArray;
    Ref<Texture2D> whiteTexture;
};

static Renderer2DStorage* m_Data;

void Renderer2D::Init() {
    FUZE_PROFILE_FUNCTION();

    m_Data = new Renderer2DStorage;

    RendererCommand::SetBlendMode(FUZE_BLEND_ALPHA);

    float vertices[4 * 5] = {
        0.5f,  0.5f,  0.0f, 1.0f, 1.0f, //
        0.5f,  -0.5f, 0.0f, 1.0f, 0.0f, //
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, //
        -0.5f, 0.5f,  0.0f, 0.0f, 1.0f, //
    };

    Ref<VertexBuffer> vertexBuffer = VertexBuffer::Create(vertices, sizeof(vertices));

    m_Data->vertexArray = VertexArray::Create();

    BufferLayout layout = {
        {ShaderDataType::Float3, "a_Position"},
        {ShaderDataType::Float2, "a_TexCoord"},
    };

    vertexBuffer->SetLayout(layout);
    m_Data->vertexArray->AddVertexBuffer(vertexBuffer);

    uint32_t indices[6] = {
        0,
        1,
        2,
        2,
        3,
        0,
    };
    Ref<IndexBuffer> indexBuffer = IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));
    m_Data->vertexArray->SetIndexBuffer(indexBuffer);

    m_Data->shader = Shader::Create(FileUtils::GetAppAsset("shaders/shader.glsl"));
    m_Data->whiteTexture = Texture2D::Create();
    m_Data->shader->SetInt("u_Texture", 0);
}

void Renderer2D::Shutdown() {
    FUZE_PROFILE_FUNCTION();

    delete m_Data;
}

void Renderer2D::BeginScene(Ref<OrthographicCamera> camera) {
    FUZE_PROFILE_FUNCTION();

    m_Data->shader->Bind();
    m_Data->shader->SetMat4("u_ViewProjection", camera->GetViewProjection());
}

void Renderer2D::EndScene() {
    FUZE_PROFILE_FUNCTION();
}

void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& scale, const glm::vec4& color, float rotation) {
    DrawQuad({position.x, position.y, 0.0f}, scale, color, rotation);
}
void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& scale, const glm::vec4& color, float rotation) {
    FUZE_PROFILE_FUNCTION();

    glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
                          glm::rotate(glm::mat4(1.0f), glm::radians(rotation), {0.0f, 0.0f, 1.0f}) *
                          glm::scale(glm::mat4(1.0f), glm::vec3(scale, 1.0f));

    m_Data->vertexArray->Bind();
    m_Data->shader->Bind();
    m_Data->shader->SetMat4("u_ModelMatrix", transform);
    m_Data->shader->SetFloat4("u_Color", color);
    m_Data->whiteTexture->Bind();
    RendererCommand::DrawIndexed(m_Data->vertexArray);
}

void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& scale, Ref<Texture2D> texture, float rotation) {
    DrawQuad({position.x, position.y, 0.0f}, scale, texture, rotation);
}
void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& scale, Ref<Texture2D> texture, float rotation) {
    FUZE_PROFILE_FUNCTION();

    glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
                          glm::rotate(glm::mat4(1.0f), glm::radians(rotation), {0.0f, 0.0f, 1.0f}) *
                          glm::scale(glm::mat4(1.0f), glm::vec3(scale, 1.0f));

    m_Data->vertexArray->Bind();
    m_Data->shader->Bind();
    m_Data->shader->SetMat4("u_ModelMatrix", transform);
    m_Data->shader->SetFloat4("u_Color", glm::vec4(1.0f));
    texture->Bind();
    RendererCommand::DrawIndexed(m_Data->vertexArray);
}

void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& scale, Ref<Texture2D> texture,
                          const glm::vec4& color, float rotation) {
    DrawQuad({position.x, position.y, 0.0f}, scale, texture, color, rotation);
}

void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& scale, Ref<Texture2D> texture,
                          const glm::vec4& color, float rotation) {
    FUZE_PROFILE_FUNCTION();

    glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
                          glm::rotate(glm::mat4(1.0f), glm::radians(rotation), {0.0f, 0.0f, 1.0f}) *
                          glm::scale(glm::mat4(1.0f), glm::vec3(scale, 1.0f));

    m_Data->vertexArray->Bind();
    m_Data->shader->Bind();
    m_Data->shader->SetMat4("u_ModelMatrix", transform);
    m_Data->shader->SetFloat4("u_Color", color);
    texture->Bind();
    RendererCommand::DrawIndexed(m_Data->vertexArray);
}
}
