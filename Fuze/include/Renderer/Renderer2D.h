#pragma once
#include "Renderer/Renderer.h"
#include "Renderer/Texture.h"
#include "Renderer/SubTexture2D.h"

namespace Fuze {
class FUZE_API Renderer2D : public Renderer {
  public:
    struct Statistics {
        int DrawCalls = 0;
        int QuadCount = 0;
    };

  public:
    static void Init();
    static void Shutdown();

    static void BeginScene(Ref<OrthographicCamera> camera);
    static void EndScene();

    static void DrawQuad(const glm::vec2& position, const glm::vec2& scale, const glm::vec4& color);
    static void DrawQuad(const glm::vec3& position, const glm::vec2& scale, const glm::vec4& color);
    static void DrawQuad(const glm::vec2& position, const glm::vec2& scale, Ref<Texture2D> texture, float tilingFactor = 1.0f,
                         const glm::vec4& color = {1.0f, 1.0f, 1.0f, 1.0f});
    static void DrawQuad(const glm::vec3& position, const glm::vec2& scale, Ref<Texture2D> texture, float tilingFactor = 1.0f,
                         const glm::vec4& color = {1.0f, 1.0f, 1.0f, 1.0f});
    static void DrawQuad(const glm::vec2& position, const glm::vec2& scale, Ref<SubTexture2D> subtexture,
                         const glm::vec4& color = {1.0f, 1.0f, 1.0f, 1.0f});
    static void DrawQuad(const glm::vec3& position, const glm::vec2& scale, Ref<SubTexture2D> subtexture,
                         const glm::vec4& color = {1.0f, 1.0f, 1.0f, 1.0f});

    // Rotation in radians
    // Rotation is slower because use Matrix multiplication
    static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& scale, float rotation, const glm::vec4& color);
    static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& scale, float rotation, const glm::vec4& color);
    static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& scale, float rotation, Ref<Texture2D> texture, float tilingFactor = 1.0f,
                                const glm::vec4& color = {1.0f, 1.0f, 1.0f, 1.0f});
    static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& scale, float rotation, Ref<Texture2D> texture, float tilingFactor = 1.0f,
                                const glm::vec4& color = {1.0f, 1.0f, 1.0f, 1.0f});
    static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& scale, float rotation, Ref<SubTexture2D> subtexture,
                                const glm::vec4& color = {1.0f, 1.0f, 1.0f, 1.0f});
    static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& scale, float rotation, Ref<SubTexture2D> subtexture,
                                const glm::vec4& color = {1.0f, 1.0f, 1.0f, 1.0f});

    static Statistics GetStats();
    static void ResetStats();

  private:
    static void Flush();
    static void FlushAndReset();
};
}
