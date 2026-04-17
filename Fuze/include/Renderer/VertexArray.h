#pragma once

#include "Renderer/Buffer.h"

namespace Fuze {

class FUZE_API VertexArray {
  public:
    static VertexArray* Create();

    virtual ~VertexArray();

    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;

    virtual void AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) = 0;
    virtual void SetIndexBuffer(const Ref<IndexBuffer>& IndexBuffer) = 0;

    virtual const std::vector<Ref<VertexBuffer>>& GetVertexBuffer() const = 0;
    virtual const Ref<IndexBuffer>& GetIndexBuffer() const = 0;

  private:
    std::vector<Ref<VertexBuffer>> m_VertexBuffers;
    Ref<IndexBuffer> m_IndexBuffer;
};
}
