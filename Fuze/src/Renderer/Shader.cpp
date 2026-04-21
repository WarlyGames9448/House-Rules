#include "fuzepch.h"
#include "Renderer/Shader.h"

#include "Renderer/Renderer.h"
#include "Plataform/OpenGL/OpenGLShader.h"

namespace Fuze {

Ref<Shader> Shader::Create(const std::string& name, const std::string& filepath) {

    switch (Renderer::GetAPI()) {
    case RendererAPI::API::none: {
        FUZE_CORE_ASSERT(false, "Renderer::none not defined!")
        return nullptr;
    }

    case RendererAPI::API::OpenGL: {
        return std::make_shared<OpenGLShader>(name, filepath);
    }
    }

    FUZE_CORE_ASSERT(false, "RendererAPI not defined!")
    return nullptr;
}

Ref<Shader> Shader::Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc) {

    switch (Renderer::GetAPI()) {
    case RendererAPI::API::none: {
        FUZE_CORE_ASSERT(false, "Renderer::none not defined!")
        return nullptr;
    }

    case RendererAPI::API::OpenGL: {
        return std::make_shared<OpenGLShader>(name, vertexSrc, fragmentSrc);
    }
    }

    FUZE_CORE_ASSERT(false, "RendererAPI not defined!")
    return nullptr;
}

void ShaderLibrary::AddShader(const std::string& name, const std::string& filepath) {
    if (NameExists(name)) {
        FUZE_CORE_ASSERT(false, "Shader name already exists: {0}", name);
        return;
    }
    m_Shaders[name] = Shader::Create(name, filepath);
}

void ShaderLibrary::AddShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc) {
    if (NameExists(name)) {
        FUZE_CORE_ASSERT(false, "Shader name already exists: {0}", name);
        return;
    }
    m_Shaders[name] = Shader::Create(name, vertexSrc, fragmentSrc);
}

Ref<Shader> ShaderLibrary::GetShader(const std::string& name) const {
     if (!NameExists(name)) {
        FUZE_CORE_ASSERT(false, "Shader name doesn't exists: {0}", name);
        return nullptr;
    }
    return m_Shaders.at(name);
}

bool ShaderLibrary::NameExists(const std::string& name) const {
    return m_Shaders.count(name);
}
}
