#include "fuzepch.h"

#include "Renderer/Renderer.h"
#include "Renderer/RendererCommand.h"

namespace Fuze {

void Renderer::BeginScene(std::shared_ptr<Shader>& shader, OrthographicCamera* camera) {
    shader->Bind();
    shader->setUniformMat4("u_ViewProjection", camera->GetViewProjection());
}
void Renderer::EndScene() {}

void Renderer::Submit(const std::shared_ptr<VertexArray>& vertexArray) { RendererCommand::DrawIndexed(vertexArray); }
}
