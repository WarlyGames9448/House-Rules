#include "fuzepch.h"
#include "Plataform/OpenGL/OpenGLRendererAPI.h"

#include <glad/glad.h>

namespace Fuze {
void OpenGLRendererAPI::SetClearColor(const glm::vec4& color) { glClearColor(color.r, color.g, color.b, color.a); }
void OpenGLRendererAPI::Clear() { glClear(GL_DEPTH_BUFFER_BIT); }

void OpenGLRendererAPI::DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray) {
    vertexArray->Bind();
    glDrawArrays(GL_TRIANGLES, 0, vertexArray->GetIndexBuffer()->GetCount());
}
}
