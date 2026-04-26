#include "fuzepch.h"

#include "Plataform/OpenGL/OpenGLShader.h"

#include "Renderer/Renderer.h"
#include "Renderer/RendererCommand.h"

namespace Fuze {

void Renderer::BeginScene(Ref<Shader>& shader, Ref<OrthographicCamera> camera) {
    std::dynamic_pointer_cast<OpenGLShader>(shader)->Bind();
    shader->SetMat4("u_ViewProjection", camera->GetViewProjection());
}

void Renderer::EndScene() {
}

void Renderer::Submit(Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& modelMatrix) {
    std::dynamic_pointer_cast<OpenGLShader>(shader)->Bind();
    shader->SetMat4("u_ModelMatrix", modelMatrix);
    RendererCommand::DrawIndexed(vertexArray);
}
}
