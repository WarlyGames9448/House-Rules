#include "fuzepch.h"

#include "Renderer/Renderer.h"
#include "Renderer/RendererCommand.h"

namespace Fuze {

void Renderer::BeginScene(std::shared_ptr<Shader>& shader, OrthographicCamera* camera) {
    shader->Bind();
    shader->setUniformMat4("u_ViewProjection", camera->GetViewProjection());
}
void Renderer::EndScene() {}

void Renderer::Submit(std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray,
                      const glm::mat4& modelMatrix) {
    shader->Bind();
    shader->setUniformMat4("u_ModelMatrix", modelMatrix);
    RendererCommand::DrawIndexed(vertexArray);
}
}
