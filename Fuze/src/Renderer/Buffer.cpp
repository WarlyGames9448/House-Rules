#include "fuzepch.h"

#include "Renderer/Buffer.h"
#include "Renderer/Renderer.h"

#include "Plataform/OpenGL/OpenGLBuffer.h"

namespace Fuze {
    VertexBuffer* VertexBuffer::Create(float* vertices, uint32_t size) {

        switch (Renderer::GetAPI()) {
        case RendererAPI::API::none: {
            FUZE_CORE_ASSERT(false, "Renderer::none not defined!")
            return nullptr;
        }

        case RendererAPI::API::OpenGL: {
            return new OpenGLVertexBuffer(vertices, size);
        }
        }

        FUZE_CORE_ASSERT(false, "RendererAPI not defined!")
        return nullptr;
    }

    IndexBuffer* IndexBuffer::Create(uint32_t* indices, uint32_t count) {

        switch (Renderer::GetAPI()) {
        case RendererAPI::API::none: {
            FUZE_CORE_ASSERT(false, "Renderer::none not defined!")
            return nullptr;
        }

        case RendererAPI::API::OpenGL: {
            return new OpenGLIndexBuffer(indices, count);
        }
        }

        FUZE_CORE_ASSERT(false, "RendererAPI not defined!")
        return nullptr;
    }
}
