#pragma once

#include "RendererAPI.h"

namespace Fuze {
class FUZE_API RendererCommand {
  public:
    inline static void SetBlendMode(BlendMode mode) {
        s_RendererAPI->SetBlendMode(mode);
    }

    inline static void DrawIndexed(const Ref<VertexArray>& vertexArray) {
        s_RendererAPI->DrawIndexed(vertexArray);
    }

    inline static void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount) {
        s_RendererAPI->DrawIndexed(vertexArray, indexCount);
    }

    inline static void DrawIndexedInstanced(const Ref<VertexArray>& vertexArray, int instanceCount){
        s_RendererAPI->DrawIndexedInstanced(vertexArray, instanceCount);
    }

    inline static void SetClearColor(const glm::vec4& color) {
        s_RendererAPI->SetClearColor(color);
    }
    inline static void Clear() {
        s_RendererAPI->Clear();
    }

    inline static const RenderCaps GetRenderCaps() {
        return s_RendererAPI->GetRenderCaps();
    }

    inline static void InitializeRenderCaps() {
        s_RendererAPI->InitializeRenderCaps();
    }

  private:
    static Scope<RendererAPI> s_RendererAPI;
};
}
