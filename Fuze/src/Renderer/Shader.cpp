#include "fuzepch.h"
#include "Renderer/Shader.h"

#include "Renderer/Renderer.h"
#include "Plataform/OpenGL/OpenGLShader.h"

namespace Fuze {

Shader* Shader::Create(const std::string& filepath) {

    switch (Renderer::GetAPI()) {
    case RendererAPI::API::none: {
        FUZE_CORE_ASSERT(false, "Renderer::none not defined!")
        return nullptr;
    }

    case RendererAPI::API::OpenGL: {
        return new OpenGLShader(filepath);
    }
    }

    FUZE_CORE_ASSERT(false, "RendererAPI not defined!")
    return nullptr;
}

Shader* Shader::Create(const std::string& vertexSrc, const std::string& fragmentSrc) {

    switch (Renderer::GetAPI()) {
    case RendererAPI::API::none: {
        FUZE_CORE_ASSERT(false, "Renderer::none not defined!")
        return nullptr;
    }

    case RendererAPI::API::OpenGL: {
        return new OpenGLShader(vertexSrc, fragmentSrc);
    }
    }

    FUZE_CORE_ASSERT(false, "RendererAPI not defined!")
    return nullptr;
}

}
