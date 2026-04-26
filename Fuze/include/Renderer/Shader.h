#pragma once

#include "Core.h"
#include <unordered_map>

namespace Fuze {

class FUZE_API Shader {
  public:
    static Ref<Shader> Create(const std::string& filepath);
    static Ref<Shader> Create(const std::string& vertexSrc, const std::string& fragmentSrc);
    virtual ~Shader() = default;

    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;

    virtual void SetInt(const std::string& name, int value) = 0;
    virtual void SetFloat4(const std::string& name, const glm::vec4& value) = 0;
    virtual void SetMat4(const std::string& name, const glm::mat4& value) = 0;

    virtual uint32_t GetRendererID() const = 0;
};

class FUZE_API ShaderLibrary {
  public:
    void AddShader(const std::string& name, const std::string& filepath);
    void AddShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
    Ref<Shader> GetShader(const std::string& name) const;

    bool NameExists(const std::string& name) const;

  private:
    std::unordered_map<std::string, Ref<Shader>> m_Shaders;
};
}
