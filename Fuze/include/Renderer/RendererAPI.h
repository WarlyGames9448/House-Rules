#pragma once

#include "Renderer/VertexArray.h"
#include "Renderer/Texture.h"

namespace Fuze {

struct RenderCaps {
    // Device Info
    std::string Vendor;
    std::string Renderer;
    std::string GraphicsAPI;

    // Texture Limits
    uint32_t MaxTextureSize = 0;
    uint32_t MaxTextureArrayLayers = 0;
    uint32_t MaxTextureImageUnits = 0;

    // Buffer & Shader Limits
    uint32_t MaxVertexAttributes = 0;
    uint32_t MaxUniformBlockSize = 0;
    uint32_t MaxColorAttachments = 0;
};

enum BlendMode {
    FUZE_BLEND_DEFAULT = 0,
    FUZE_BLEND_ALPHA,
};

class FUZE_API RendererAPI {
  public:
    enum class API {
        none = 0,
        OpenGL = 1,
    };

  public:
    inline static API GetAPI() {
        return s_API;
    }

    virtual const RenderCaps& GetRenderCaps() = 0;

    virtual void InitializeRenderCaps() = 0;

    virtual void SetBlendMode(BlendMode mode) = 0;

    virtual void SetClearColor(const glm::vec4& color) = 0;
    virtual void Clear() = 0;

    virtual void DrawIndexed(const Ref<VertexArray>& vertexArray) = 0;

    // When indexCount has the possibility to be zero
    virtual void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount) = 0;

    virtual void DrawIndexedInstanced(const Ref<VertexArray>& vertexArray, int instancedCount) = 0;

  private:
    static API s_API;
};
}
