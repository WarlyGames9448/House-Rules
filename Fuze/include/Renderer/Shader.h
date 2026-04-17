#pragma once

#include "Core.h"

namespace Fuze {

class FUZE_API Shader {
  public:
    static Shader* Create(const std::string& vertexSrc, const std::string& fragmentSrc);
    virtual ~Shader() = default;

    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;

    inline uint32_t GetRendererID() {
        return m_RendererID;
    }

  private:
    uint32_t m_RendererID;
};
}
