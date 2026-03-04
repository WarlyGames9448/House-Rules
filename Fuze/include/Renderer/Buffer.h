#pragma once

namespace Fuze {

    class FUZE_API VertexBuffer {
      public:
        static VertexBuffer* Create(float* vertices, uint32_t size);

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;
    };

    class FUZE_API IndexBuffer {
      public:
        static IndexBuffer* Create(uint32_t* indices, uint32_t count);

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;
    };
}
