#include "fuzepch.h"

#include "Renderer/Framebuffer.h"
#include "Renderer/Renderer.h"

#include "Plataform/OpenGL/OpenGLFramebuffer.h"

namespace Fuze {
Ref<Framebuffer> Framebuffer::Create(const FramebufferSpecification& spec) {

    switch (Renderer::GetAPI()) {
    case RendererAPI::API::none: {
        FUZE_CORE_ASSERT(false, "Renderer::none not defined!")
        return nullptr;
    }

    case RendererAPI::API::OpenGL: {
        return CreateRef<OpenGLFramebuffer>(spec);
    }
    }

    FUZE_CORE_ASSERT(false, "RendererAPI not defined!")
    return nullptr;
}
}
