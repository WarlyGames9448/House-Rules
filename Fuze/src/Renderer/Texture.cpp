#include "fuzepch.h"

#include "Renderer/Texture.h"

#include "Renderer/Renderer.h"

#include "Plataform/OpenGL/OpenGLTexture.h"
#include <memory>

namespace Fuze {

Ref<Texture2D> Texture2D::Create() {

    switch (Renderer::GetAPI()) {
    case RendererAPI::API::none: {
        FUZE_CORE_ASSERT(false, "Renderer::none not defined!")
        return nullptr;
    }

    case RendererAPI::API::OpenGL: {
        return CreateRef<OpenGLTexture2D>();
    }
    }

    FUZE_CORE_ASSERT(false, "RendererAPI not defined!")
    return nullptr;
}

Ref<Texture2D> Texture2D::Create(const std::string& path) {

    switch (Renderer::GetAPI()) {
    case RendererAPI::API::none: {
        FUZE_CORE_ASSERT(false, "Renderer::none not defined!")
        return nullptr;
    }

    case RendererAPI::API::OpenGL: {
        return CreateRef<OpenGLTexture2D>(path);
    }
    }

    FUZE_CORE_ASSERT(false, "RendererAPI not defined!")
    return nullptr;
}
}
