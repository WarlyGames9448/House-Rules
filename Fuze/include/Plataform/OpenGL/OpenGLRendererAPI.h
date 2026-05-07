#pragma once

#include "Renderer/RendererAPI.h"

namespace Fuze {
class OpenGLRendererAPI : public RendererAPI {
  public:
    virtual void SetBlendMode(BlendMode mode) override;

    virtual void SetClearColor(const glm::vec4& color) override;
    virtual void Clear() override;

    virtual void DrawIndexed(const Ref<VertexArray>& vertexArray) override;
    virtual void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount) override;
};
}
