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
    float TexIndex;
};

struct Renderer2DData {
    const uint32_t maxQuads = 10000;
    const uint32_t maxVertices = maxQuads * 4;
    const uint32_t maxIndices = maxQuads * 6;

    Ref<Shader> shader;
    Ref<VertexBuffer> vertexBuffer;
    Ref<VertexArray> vertexArray;
    Ref<Texture2D> whiteTexture;

    uint32_t quadIndexCount = 0;

    QuadVertex* quadVertexBufferBase = nullptr;
    QuadVertex* quadVertexBufferPtr = nullptr;

    int maxTextureSlots = 32;

    Ref<Texture2D> TextureSlots[32];
    int TextureSlotIndex = 1;

    const glm::vec4 VertexPositions[4] = {
        { 0.5f,  0.5f, 0.0f, 1.0f},
        { 0.5f, -0.5f, 0.0f, 1.0f},
        {-0.5f, -0.5f, 0.0f, 1.0f},
        {-0.5f,  0.5f, 0.0f, 1.0f}
    };
};

static Renderer2DData s_Data;

static Renderer2D::Statistics stats;

void Renderer2D::Init() {
    FUZE_PROFILE_FUNCTION();

    RendererCommand::SetBlendMode(FUZE_BLEND_ALPHA);

    s_Data.vertexBuffer = VertexBuffer::Create(s_Data.maxVertices * sizeof(QuadVertex));
    s_Data.vertexBuffer->SetLayout({
        {ShaderDataType::Float3, "a_Position"},
        {ShaderDataType::Float4,    "a_Color"},
        {ShaderDataType::Float2, "a_TexCoord"},
        { ShaderDataType::Float, "a_TexIndex"}
    });

    s_Data.vertexArray = VertexArray::Create();
    s_Data.vertexArray->AddVertexBuffer(s_Data.vertexBuffer);

    s_Data.quadVertexBufferBase = new QuadVertex[s_Data.maxVertices];

    uint32_t* quadIndices = new uint32_t[s_Data.maxIndices];

    uint32_t offset = 0;
    for (uint32_t i = 0; i < s_Data.maxIndices; i += 6) {
        quadIndices[i + 0] = offset + 0;
        quadIndices[i + 1] = offset + 1;
        quadIndices[i + 2] = offset + 2;

        quadIndices[i + 3] = offset + 2;
        quadIndices[i + 4] = offset + 3;
        quadIndices[i + 5] = offset + 0;

        offset += 4;
    }

    Ref<IndexBuffer> quadIB = IndexBuffer::Create(quadIndices, s_Data.maxIndices);
    s_Data.vertexArray->SetIndexBuffer(quadIB);
    delete[] quadIndices;

    s_Data.shader = Shader::Create(FileUtils::GetAppAsset("shaders/shader.glsl"));
    s_Data.whiteTexture = Texture2D::Create();

    // Max supported is 32 textures
    s_Data.maxTextureSlots = RendererCommand::GetRenderCaps().MaxTextureImageUnits <= 32 ? RendererCommand::GetRenderCaps().MaxTextureImageUnits : 32;

    s_Data.shader->Bind();
    int samplers[32];
    for (int i = 0; i < 32; i++)
        samplers[i] = i;

    // NOTE: Some hardware doent support dynamic sampler indexing, so we
    // use switch case in fragment shader.
    s_Data.shader->SetIntArray("u_Textures", 32, samplers);
    s_Data.TextureSlots[0] = s_Data.whiteTexture;
}

void Renderer2D::Shutdown() {
    FUZE_PROFILE_FUNCTION();
}

void Renderer2D::BeginScene(Ref<OrthographicCamera> camera) {
    FUZE_PROFILE_FUNCTION();

    s_Data.shader->Bind();
    s_Data.shader->SetMat4("u_ViewProjection", camera->GetViewProjection());

    s_Data.TextureSlotIndex = 1;
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
    for (int i = 0; i < s_Data.TextureSlotIndex; i++) {
        s_Data.TextureSlots[i]->Bind(i);
    }

    RendererCommand::DrawIndexed(s_Data.vertexArray, s_Data.quadIndexCount);
    stats.DrawCalls++;
}

void Renderer2D::FlushAndReset() {
    uint32_t dataSize = (uint8_t*)s_Data.quadVertexBufferPtr - (uint8_t*)s_Data.quadVertexBufferBase;
    s_Data.vertexBuffer->SetData(s_Data.quadVertexBufferBase, dataSize);

    Flush();

    s_Data.TextureSlotIndex = 1;
    s_Data.quadIndexCount = 0;

    s_Data.quadVertexBufferPtr = s_Data.quadVertexBufferBase;
}

void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& scale, const glm::vec4& color) {
    DrawQuad({position.x, position.y, 0.0f}, scale, color);
}

void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& scale, const glm::vec4& color) {
    FUZE_PROFILE_FUNCTION();

    s_Data.quadVertexBufferPtr->Position = position;
    s_Data.quadVertexBufferPtr->Color = color;
    s_Data.quadVertexBufferPtr->TexCoords = {0.0f, 0.0f};
    s_Data.quadVertexBufferPtr->TexIndex = 0.0f;
    s_Data.quadVertexBufferPtr++;

    s_Data.quadVertexBufferPtr->Position = {position.x + scale.x, position.y, 1.0f};
    s_Data.quadVertexBufferPtr->Color = color;
    s_Data.quadVertexBufferPtr->TexCoords = {1.0f, 0.0f};
    s_Data.quadVertexBufferPtr->TexIndex = 0.0f;
    s_Data.quadVertexBufferPtr++;

    s_Data.quadVertexBufferPtr->Position = {position.x + scale.x, position.y + scale.y, 1.0f};
    s_Data.quadVertexBufferPtr->Color = color;
    s_Data.quadVertexBufferPtr->TexCoords = {1.0f, 1.0f};
    s_Data.quadVertexBufferPtr->TexIndex = 0.0f;
    s_Data.quadVertexBufferPtr++;

    s_Data.quadVertexBufferPtr->Position = {position.x, position.y + scale.y, 1.0f};
    s_Data.quadVertexBufferPtr->Color = color;
    s_Data.quadVertexBufferPtr->TexCoords = {0.0f, 1.0f};
    s_Data.quadVertexBufferPtr->TexIndex = 0.0f;
    s_Data.quadVertexBufferPtr++;

    s_Data.quadIndexCount += 6;

    stats.QuadCount++;
}

void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& scale, Ref<Texture2D> texture, float tilingFactor, const glm::vec4& color) {
    DrawQuad({position.x, position.y, 0.0f}, scale, texture, tilingFactor, color);
}

void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& scale, Ref<Texture2D> texture, float tilingFactor, const glm::vec4& color) {
    FUZE_PROFILE_FUNCTION();

    // Finding index of the texture
    float textureIndex = 0.0f;
    for (int index = 0; index < s_Data.TextureSlotIndex; index++) {
        if (s_Data.TextureSlots[index]->GetRendererID() == texture->GetRendererID()) {
            textureIndex = (float)index;
            break;
        }
    }

    if (!textureIndex) {
        if (s_Data.TextureSlotIndex > s_Data.maxTextureSlots) {
            FlushAndReset();
        }

        textureIndex = s_Data.TextureSlotIndex;
        s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
        s_Data.TextureSlotIndex++;
    }

    s_Data.quadVertexBufferPtr->Position = position;
    s_Data.quadVertexBufferPtr->Color = color;
    s_Data.quadVertexBufferPtr->TexCoords = {0.0f, 0.0f};
    s_Data.quadVertexBufferPtr->TexIndex = textureIndex;
    s_Data.quadVertexBufferPtr++;

    s_Data.quadVertexBufferPtr->Position = {position.x + scale.x, position.y, 1.0f};
    s_Data.quadVertexBufferPtr->Color = color;
    s_Data.quadVertexBufferPtr->TexCoords = {1.0f * tilingFactor, 0.0f};
    s_Data.quadVertexBufferPtr->TexIndex = textureIndex;
    s_Data.quadVertexBufferPtr++;

    s_Data.quadVertexBufferPtr->Position = {position.x + scale.x, position.y + scale.y, 1.0f};
    s_Data.quadVertexBufferPtr->Color = color;
    s_Data.quadVertexBufferPtr->TexCoords = {1.0f * tilingFactor, 1.0f * tilingFactor};
    s_Data.quadVertexBufferPtr->TexIndex = textureIndex;
    s_Data.quadVertexBufferPtr++;

    s_Data.quadVertexBufferPtr->Position = {position.x, position.y + scale.y, 1.0f};
    s_Data.quadVertexBufferPtr->Color = color;
    s_Data.quadVertexBufferPtr->TexCoords = {0.0f, 1.0f * tilingFactor};
    s_Data.quadVertexBufferPtr->TexIndex = textureIndex;
    s_Data.quadVertexBufferPtr++;
    s_Data.quadIndexCount += 6;

    stats.QuadCount++;
}

void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& scale, Ref<SubTexture2D> subtexture,
                          const glm::vec4& color) {
    DrawQuad({position.x, position.y, 0.0f}, scale, subtexture, color);
}
void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& scale, Ref<SubTexture2D> subtexture,
                          const glm::vec4& color) {
    FUZE_PROFILE_FUNCTION();

    Ref<Texture2D> texture = subtexture->GetTexture();
    glm::vec2* texCoords = subtexture->GetTexCoords();

    // Finding index of the texture
    float textureIndex = 0.0f;
    for (int index = 0; index < s_Data.TextureSlotIndex; index++) {
        if (s_Data.TextureSlots[index]->GetRendererID() == texture->GetRendererID()) {
            textureIndex = (float)index;
            break;
        }
    }

    if (!textureIndex) {
        if (s_Data.TextureSlotIndex > s_Data.maxTextureSlots) {
            FlushAndReset();
        }

        textureIndex = s_Data.TextureSlotIndex;
        s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
        s_Data.TextureSlotIndex++;
    }

    s_Data.quadVertexBufferPtr->Position = position;
    s_Data.quadVertexBufferPtr->Color = color;
    s_Data.quadVertexBufferPtr->TexCoords = texCoords[0];
    s_Data.quadVertexBufferPtr->TexIndex = textureIndex;
    s_Data.quadVertexBufferPtr++;

    s_Data.quadVertexBufferPtr->Position = {position.x + scale.x, position.y, 1.0f};
    s_Data.quadVertexBufferPtr->Color = color;
    s_Data.quadVertexBufferPtr->TexCoords = texCoords[1];
    s_Data.quadVertexBufferPtr->TexIndex = textureIndex;
    s_Data.quadVertexBufferPtr++;

    s_Data.quadVertexBufferPtr->Position = {position.x + scale.x, position.y + scale.y, 1.0f};
    s_Data.quadVertexBufferPtr->Color = color;
    s_Data.quadVertexBufferPtr->TexCoords = texCoords[2];
    s_Data.quadVertexBufferPtr->TexIndex = textureIndex;
    s_Data.quadVertexBufferPtr++;

    s_Data.quadVertexBufferPtr->Position = {position.x, position.y + scale.y, 1.0f};
    s_Data.quadVertexBufferPtr->Color = color;
    s_Data.quadVertexBufferPtr->TexCoords = texCoords[3];
    s_Data.quadVertexBufferPtr->TexIndex = textureIndex;
    s_Data.quadVertexBufferPtr++;
    s_Data.quadIndexCount += 6;

    stats.QuadCount++;
}

void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& scale, float rotation, const glm::vec4& color) {
    DrawRotatedQuad({position.x, position.y, 0.0f}, scale, rotation, color);
}

void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& scale, float rotation, const glm::vec4& color) {
    FUZE_PROFILE_FUNCTION();

    glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::rotate(glm::mat4(1.0f), rotation, {0.0f, 0.0f, 1.0f}) *
                          glm::scale(glm::mat4(1.0f), {scale, 1.0f});

    s_Data.quadVertexBufferPtr->Position = transform * s_Data.VertexPositions[0];
    s_Data.quadVertexBufferPtr->Color = color;
    s_Data.quadVertexBufferPtr->TexCoords = {0.0f, 0.0f};
    s_Data.quadVertexBufferPtr->TexIndex = 0.0f;
    s_Data.quadVertexBufferPtr++;

    s_Data.quadVertexBufferPtr->Position = transform * s_Data.VertexPositions[1];
    s_Data.quadVertexBufferPtr->Color = color;
    s_Data.quadVertexBufferPtr->TexCoords = {1.0f, 0.0f};
    s_Data.quadVertexBufferPtr->TexIndex = 0.0f;
    s_Data.quadVertexBufferPtr++;

    s_Data.quadVertexBufferPtr->Position = transform * s_Data.VertexPositions[2];
    s_Data.quadVertexBufferPtr->Color = color;
    s_Data.quadVertexBufferPtr->TexCoords = {1.0f, 1.0f};
    s_Data.quadVertexBufferPtr->TexIndex = 0.0f;
    s_Data.quadVertexBufferPtr++;

    s_Data.quadVertexBufferPtr->Position = transform * s_Data.VertexPositions[3];
    s_Data.quadVertexBufferPtr->Color = color;
    s_Data.quadVertexBufferPtr->TexCoords = {0.0f, 1.0f};
    s_Data.quadVertexBufferPtr->TexIndex = 0.0f;
    s_Data.quadVertexBufferPtr++;

    s_Data.quadIndexCount += 6;

    stats.QuadCount++;
}

void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& scale, float rotation, Ref<Texture2D> texture, float tilingFactor,
                                 const glm::vec4& color) {
    DrawRotatedQuad({position.x, position.y, 0.0f}, scale, rotation, texture, tilingFactor, color);
}

void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& scale, float rotation, Ref<Texture2D> texture, float tilingFactor,
                                 const glm::vec4& color) {
    FUZE_PROFILE_FUNCTION();

    // Finding index of the texture
    float textureIndex = 0.0f;
    for (int index = 0; index < s_Data.TextureSlotIndex; index++) {
        if (s_Data.TextureSlots[index]->GetRendererID() == texture->GetRendererID()) {
            textureIndex = (float)index;
            break;
        }
    }

    if (!textureIndex) {
        if (s_Data.TextureSlotIndex > s_Data.maxTextureSlots) {
            FlushAndReset();
        }

        textureIndex = s_Data.TextureSlotIndex;
        s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
        s_Data.TextureSlotIndex++;
    }

    glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::rotate(glm::mat4(1.0f), rotation, {0.0f, 0.0f, 1.0f}) *
                          glm::scale(glm::mat4(1.0f), {scale, 1.0f});

    s_Data.quadVertexBufferPtr->Position = transform * s_Data.VertexPositions[0];
    s_Data.quadVertexBufferPtr->Color = color;
    s_Data.quadVertexBufferPtr->TexCoords = {0.0f, 0.0f};
    s_Data.quadVertexBufferPtr->TexIndex = textureIndex;
    s_Data.quadVertexBufferPtr++;

    s_Data.quadVertexBufferPtr->Position = transform * s_Data.VertexPositions[1];
    s_Data.quadVertexBufferPtr->Color = color;
    s_Data.quadVertexBufferPtr->TexCoords = {1.0f * tilingFactor, 0.0f};
    s_Data.quadVertexBufferPtr->TexIndex = textureIndex;
    s_Data.quadVertexBufferPtr++;

    s_Data.quadVertexBufferPtr->Position = transform * s_Data.VertexPositions[2];
    s_Data.quadVertexBufferPtr->Color = color;
    s_Data.quadVertexBufferPtr->TexCoords = {1.0f * tilingFactor, 1.0f * tilingFactor};
    s_Data.quadVertexBufferPtr->TexIndex = textureIndex;
    s_Data.quadVertexBufferPtr++;

    s_Data.quadVertexBufferPtr->Position = transform * s_Data.VertexPositions[3];
    s_Data.quadVertexBufferPtr->Color = color;
    s_Data.quadVertexBufferPtr->TexCoords = {0.0f, 1.0f * tilingFactor};
    s_Data.quadVertexBufferPtr->TexIndex = textureIndex;
    s_Data.quadVertexBufferPtr++;
    s_Data.quadIndexCount += 6;

    stats.QuadCount++;
}

void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& scale, float rotation, Ref<SubTexture2D> subtexture,
                                 const glm::vec4& color) {
    DrawRotatedQuad({position.x, position.y, 0.0f}, scale, rotation, subtexture, color);
}

void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& scale, float rotation, Ref<SubTexture2D> subtexture,
                                 const glm::vec4& color) {
    FUZE_PROFILE_FUNCTION();

    Ref<Texture2D> texture = subtexture->GetTexture();
    glm::vec2* texCoords = subtexture->GetTexCoords();

    // Finding index of the texture
    float textureIndex = 0.0f;
    for (int index = 0; index < s_Data.TextureSlotIndex; index++) {
        if (s_Data.TextureSlots[index]->GetRendererID() == texture->GetRendererID()) {
            textureIndex = (float)index;
            break;
        }
    }

    if (!textureIndex) {
        if (s_Data.TextureSlotIndex > s_Data.maxTextureSlots) {
            FlushAndReset();
        }

        textureIndex = s_Data.TextureSlotIndex;
        s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
        s_Data.TextureSlotIndex++;
    }

    glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::rotate(glm::mat4(1.0f), rotation, {0.0f, 0.0f, 1.0f}) *
                          glm::scale(glm::mat4(1.0f), {scale, 1.0f});

    s_Data.quadVertexBufferPtr->Position = transform * s_Data.VertexPositions[0];
    s_Data.quadVertexBufferPtr->Color = color;
    s_Data.quadVertexBufferPtr->TexCoords = texCoords[0];
    s_Data.quadVertexBufferPtr->TexIndex = textureIndex;
    s_Data.quadVertexBufferPtr++;

    s_Data.quadVertexBufferPtr->Position = transform * s_Data.VertexPositions[1];
    s_Data.quadVertexBufferPtr->Color = color;
    s_Data.quadVertexBufferPtr->TexCoords = texCoords[1];
    s_Data.quadVertexBufferPtr->TexIndex = textureIndex;
    s_Data.quadVertexBufferPtr++;

    s_Data.quadVertexBufferPtr->Position = transform * s_Data.VertexPositions[2];
    s_Data.quadVertexBufferPtr->Color = color;
    s_Data.quadVertexBufferPtr->TexCoords = texCoords[2];
    s_Data.quadVertexBufferPtr->TexIndex = textureIndex;
    s_Data.quadVertexBufferPtr++;

    s_Data.quadVertexBufferPtr->Position = transform * s_Data.VertexPositions[3];
    s_Data.quadVertexBufferPtr->Color = color;
    s_Data.quadVertexBufferPtr->TexCoords = texCoords[3];
    s_Data.quadVertexBufferPtr->TexIndex = textureIndex;
    s_Data.quadVertexBufferPtr++;
    s_Data.quadIndexCount += 6;

    stats.QuadCount++;
}

Renderer2D::Statistics Renderer2D::GetStats() {
    return stats;
}

void Renderer2D::ResetStats() {
    stats.DrawCalls = 0;
    stats.QuadCount = 0;
}
}
