#include "fuzepch.h"

#include "Plataform/OpenGL/OpenGLShader.h"

#include "Renderer/Renderer.h"
#include "Renderer/RendererCommand.h"

namespace Fuze {

void Renderer::BeginScene(std::shared_ptr<Shader>& shader, OrthographicCamera* camera) {
    std::dynamic_pointer_cast<OpenGLShader>(shader)->Bind();
    std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_ViewProjection", camera->GetViewProjection());
}

void Renderer::EndScene() {
}

void Renderer::Submit(std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray,
                      const glm::mat4& modelMatrix) {
    std::dynamic_pointer_cast<OpenGLShader>(shader)->Bind();
    std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_ModelMatrix", modelMatrix);
    RendererCommand::DrawIndexed(vertexArray);
}
}
