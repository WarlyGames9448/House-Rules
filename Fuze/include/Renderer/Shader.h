#pragma once

namespace Fuze {

class FUZE_API Shader {
  public:
    Shader(const std::string& vertexSrc, const std::string& fragmentSrc);
    ~Shader();

    void Bind();
    void Unbind();

    inline uint32_t GetRendererID() { return m_RendererID; }

    void setUniformMat4(glm::mat4 projection);

  private:
    uint32_t m_RendererID;
};
}
