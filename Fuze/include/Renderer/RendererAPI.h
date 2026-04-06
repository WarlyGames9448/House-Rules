#pragma once

#include "Renderer/VertexArray.h"

namespace Fuze {
class RendererAPI {
  public:
    enum class API {
        none = 0,
        OpenGL = 1,
    };

  public:
    inline static API GetAPI() { return s_API; }

    virtual void SetClearColor(const glm::vec4& color) = 0;
    virtual void Clear() = 0;

    virtual void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray) = 0;

  private:
    static API s_API;
};
}
