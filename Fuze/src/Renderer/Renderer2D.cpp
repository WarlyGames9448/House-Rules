#include "fuzepch.h"

#include "Renderer/Renderer2D.h"

#include "Renderer/RendererCommand.h"
#include <glm/gtc/matrix_transform.hpp>

#include "Utils/FileUtils.h"

namespace Fuze {

struct QuadVertex {
    glm::vec3 Position;
    glm::vec4 Color;
    glm::vec2 TexCoords;
    //int TexIndex;
};

struct Renderer2DData {
    const uint32_t MaxQuads = 10000;
    const uint32_t MaxVertices = MaxQuads * 4;
    const uint32_t MaxIndices = MaxQuads * 6;

    Ref<Shader> shader;
    Ref<VertexBuffer> vertexBuffer;
    Ref<VertexArray> vertexArray;
    Ref<Texture2D> whiteTexture;

    uint32_t quadIndexCount = 0;

    QuadVertex* quadVertexBufferBase = nullptr;
    QuadVertex* quadVertexBufferPtr = nullptr;
};

static Renderer2DData s_Data;

void Renderer2D::Init() {
    FUZE_PROFILE_FUNCTION();

    RendererCommand::SetBlendMode(FUZE_BLEND_ALPHA);

    s_Data.vertexBuffer = VertexBuffer::Create(s_Data.MaxVertices * sizeof(QuadVertex));
    s_Data.vertexBuffer->SetLayout({
        {ShaderDataType::Float3, "a_Position"},
        {ShaderDataType::Float4,    "a_Color"},
        {ShaderDataType::Float2, "a_TexCoord"},
        //{   ShaderDataType::Int, "a_TexIndex"}
    });

    s_Data.vertexArray = VertexArray::Create();
    s_Data.vertexArray->AddVertexBuffer(s_Data.vertexBuffer);

    s_Data.quadVertexBufferBase = new QuadVertex[s_Data.MaxVertices];

    uint32_t* quadIndices = new uint32_t[s_Data.MaxIndices];

    uint32_t offset = 0;
    for (uint32_t i = 0; i < s_Data.MaxIndices; i += 6) {
        quadIndices[i + 0] = offset + 0;
        quadIndices[i + 1] = offset + 1;
        quadIndices[i + 2] = offset + 2;

        quadIndices[i + 3] = offset + 2;
        quadIndices[i + 4] = offset + 3;
        quadIndices[i + 5] = offset + 0;

        offset += 4;
    }

    Ref<IndexBuffer> quadIB = IndexBuffer::Create(quadIndices, s_Data.MaxIndices);
    s_Data.vertexArray->SetIndexBuffer(quadIB);
    delete[] quadIndices;

    s_Data.shader = Shader::Create(FileUtils::GetAppAsset("shaders/shader.glsl"));
    s_Data.whiteTexture = Texture2D::Create();
    s_Data.shader->SetInt("u_Texture", 0);
}

void Renderer2D::Shutdown() {
    FUZE_PROFILE_FUNCTION();
}

void Renderer2D::BeginScene(Ref<OrthographicCamera> camera) {
    FUZE_PROFILE_FUNCTION();

    s_Data.shader->Bind();
    s_Data.shader->SetMat4("u_ViewProjection", camera->GetViewProjection());

    s_Data.quadIndexCount = 0;
    s_Data.quadVertexBufferPtr = s_Data.quadVertexBufferBase;
}

void Renderer2D::EndScene() {
    FUZE_PROFILE_FUNCTION();

    uint32_t dataSize = (uint8_t*)s_Data.quadVertexBufferPtr - (uint8_t*)s_Data.quadVertexBufferBase;
    s_Data.vertexBuffer->SetData(s_Data.quadVertexBufferBase, dataSize);

    Flush();
}

void Renderer2D::Flush() {
    RendererCommand::DrawIndexed(s_Data.vertexArray, s_Data.quadIndexCount);
}

void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& scale, const glm::vec4& color, float rotation) {
    DrawQuad({position.x, position.y, 0.0f}, scale, color, rotation);
}
void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& scale, const glm::vec4& color, float rotation) {
    FUZE_PROFILE_FUNCTION();

    s_Data.quadVertexBufferPtr->Position = position;
    s_Data.quadVertexBufferPtr->Color = color;
    s_Data.quadVertexBufferPtr->TexCoords = {0.0f, 0.0f};
    s_Data.quadVertexBufferPtr++;

    s_Data.quadVertexBufferPtr->Position = {position.x + scale.x, position.y, 1.0f};
    s_Data.quadVertexBufferPtr->Color = color;
    s_Data.quadVertexBufferPtr->TexCoords = {1.0f, 0.0f};
    s_Data.quadVertexBufferPtr++;

    s_Data.quadVertexBufferPtr->Position = {position.x + scale.x, position.y + scale.y, 1.0f};
    s_Data.quadVertexBufferPtr->Color = color;
    s_Data.quadVertexBufferPtr->TexCoords = {1.0f, 1.0f};
    s_Data.quadVertexBufferPtr++;

    s_Data.quadVertexBufferPtr->Position = {position.x, position.y + scale.y, 1.0f};
    s_Data.quadVertexBufferPtr->Color = color;
    s_Data.quadVertexBufferPtr->TexCoords = {0.0f, 1.0f};
    s_Data.quadVertexBufferPtr++;

    s_Data.quadIndexCount += 6;
}

void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& scale, Ref<Texture2D> texture, float rotation) {
    DrawQuad({position.x, position.y, 0.0f}, scale, texture, rotation);
}
void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& scale, Ref<Texture2D> texture, float rotation) {
    FUZE_PROFILE_FUNCTION();

    glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
                          glm::rotate(glm::mat4(1.0f), glm::radians(rotation), {0.0f, 0.0f, 1.0f}) *
                          glm::scale(glm::mat4(1.0f), glm::vec3(scale, 1.0f));

    s_Data.vertexArray->Bind();
    s_Data.shader->Bind();
    s_Data.shader->SetMat4("u_ModelMatrix", transform);
    s_Data.shader->SetFloat4("u_Color", glm::vec4(1.0f));
    texture->Bind();
    RendererCommand::DrawIndexed(s_Data.vertexArray);
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

    s_Data.vertexArray->Bind();
    s_Data.shader->Bind();
    s_Data.shader->SetMat4("u_ModelMatrix", transform);
    s_Data.shader->SetFloat4("u_Color", color);
    texture->Bind();
    RendererCommand::DrawIndexed(s_Data.vertexArray);
}
}
