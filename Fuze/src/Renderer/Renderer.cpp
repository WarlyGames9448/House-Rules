#include "fuzepch.h"

#include "Plataform/OpenGL/OpenGLShader.h"

#include "Renderer/Renderer.h"
#include "Renderer/RendererCommand.h"

namespace Fuze {

void Renderer::BeginScene(Ref<Shader>& shader, Ref<OrthographicCamera> camera) {
    shader->Bind();
    shader->SetMat4("u_ViewProjection", camera->GetViewProjection());
}

void Renderer::EndScene() {
}
}
