#include "fuzepch.h"

#include "Renderer/VertexArray.h"
#include "Renderer/Renderer.h"

#include "Plataform/OpenGL/OpenGLVertexArray.h"

namespace Fuze {
Ref<VertexArray> VertexArray::Create() {

    switch (Renderer::GetAPI()) {
    case RendererAPI::API::none: {
        FUZE_CORE_ASSERT(false, "Renderer::none not defined!")
        return nullptr;
    }

    case RendererAPI::API::OpenGL: {
        return CreateRef<OpenGLVertexArray>();
    }
    }

    FUZE_CORE_ASSERT(false, "RendererAPI not defined!")
    return nullptr;
}

VertexArray::~VertexArray() {}
}
