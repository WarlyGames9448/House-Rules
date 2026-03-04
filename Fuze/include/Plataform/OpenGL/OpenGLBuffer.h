#pragma once

#include "Renderer/Buffer.h"

namespace Fuze {

    class OpenGLVertexBuffer : public VertexBuffer {
      public:
        OpenGLVertexBuffer(float* vertices, uint32_t size);
        ~OpenGLVertexBuffer();

        void Bind() const;
        void Unbind() const;

      private:
        uint32_t m_RendererID;
    };

    class OpenGLIndexBuffer : public IndexBuffer {
      public:
        OpenGLIndexBuffer(uint32_t* indices, uint32_t count);
        ~OpenGLIndexBuffer();

        void Bind() const;
        void Unbind() const;

        inline uint32_t GetCount() { return m_Count; }

      private:
        uint32_t m_Count;
        uint32_t m_RendererID;
    };
}
