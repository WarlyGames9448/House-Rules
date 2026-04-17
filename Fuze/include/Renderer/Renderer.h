#pragma once

#include "Renderer/RendererAPI.h"
#include "Renderer/VertexArray.h"
#include "Renderer/Shader.h"

#include "Renderer/OrthographicCamera.h"

namespace Fuze {

class FUZE_API Renderer {
  public:
    inline static RendererAPI::API GetAPI() {
        return RendererAPI::GetAPI();
    }

    static void BeginScene(Ref<Shader>& shader, OrthographicCamera* camera);
    static void EndScene();

    static void Submit(Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& modelMatrix = 1.0f);
};
}
