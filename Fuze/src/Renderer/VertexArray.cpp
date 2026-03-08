#include "fuzepch.h"

#include "Renderer/VertexArray.h"
#include "Renderer/Renderer.h"

#include "Plataform/OpenGL/OpenGLVertexArray.h"

namespace Fuze {
    VertexArray* VertexArray::Create() {

        switch (Renderer::GetAPI()) {
        case RendererAPI::none: {
            FUZE_CORE_ASSERT(false, "Renderer::none not defined!")
            return nullptr;
        }

        case RendererAPI::OpenGL: {
            return new OpenGLVertexArray();
        }
        }

        FUZE_CORE_ASSERT(false, "RendererAPI not defined!")
        return nullptr;
    }
}
