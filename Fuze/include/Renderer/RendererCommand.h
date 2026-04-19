#pragma once

#include "RendererAPI.h"

namespace Fuze {
class RendererCommand {
  public:
    inline static void SetBlendMode(BlendMode mode){
        s_RendererAPI->SetBlendMode(mode);
    }
    inline static void DrawIndexed(const Ref<VertexArray>& vertexArray) {
        s_RendererAPI->DrawIndexed(vertexArray);
    }

    inline static void SetClearColor(const glm::vec4& color) {
        s_RendererAPI->SetClearColor(color);
    }
    inline static void Clear() {
        s_RendererAPI->Clear();
    }

  private:
    static RendererAPI* s_RendererAPI;
};
}
