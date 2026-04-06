#pragma once

#include "Renderer/Buffer.h"

namespace Fuze {

class FUZE_API VertexArray {
  public:
    static VertexArray* Create();

    virtual ~VertexArray();

    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;

    virtual void AddVertexBuffer(const std::shared_ptr<VertexBuffer> vertexBuffer) = 0;
    virtual void SetIndexBuffer(const std::shared_ptr<IndexBuffer> IndexBuffer) = 0;

    virtual std::vector<std::shared_ptr<VertexBuffer>> GetVertexBuffer() const = 0;
    virtual std::shared_ptr<IndexBuffer> GetIndexBuffer() const = 0;

  private:
    std::vector<std::shared_ptr<VertexBuffer>> m_VertexBuffers;
    std::shared_ptr<IndexBuffer> m_IndexBuffer;
};
}
