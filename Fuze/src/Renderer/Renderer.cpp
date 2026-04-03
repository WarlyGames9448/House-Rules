#include "fuzepch.h"

#include "Renderer/Renderer.h"
#include "Renderer/RendererCommand.h"

namespace Fuze {

void Renderer::BeginScene() {}
void Renderer::EndScene() {}

void Renderer::Submit(const std::shared_ptr<VertexArray>& vertexArray) { RendererCommand::DrawIndexed(vertexArray); }
}
