#pragma once
#include "Renderer/Renderer.h"
#include "Renderer/Texture.h"

namespace Fuze {
class FUZE_API Renderer2D : public Renderer {
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

  private:
    static void Flush();
    static void FlushAndReset();
};
}
