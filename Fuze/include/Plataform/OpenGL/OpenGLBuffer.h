#pragma once

#include "Renderer/Buffer.h"

namespace Fuze {

    class OpenGLVertexBuffer : public VertexBuffer {
      public:
        OpenGLVertexBuffer(float* vertices, uint32_t size);
        ~OpenGLVertexBuffer();

        void Bind() const override;
        void Unbind() const override;

        virtual const BufferLayout& GetLayout() const override { return m_Layout; }
        virtual void SetLayout(const BufferLayout& layout) override { m_Layout = layout; }

      private:
        uint32_t m_RendererID;
        BufferLayout m_Layout;
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
