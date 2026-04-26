#pragma once

#include "Renderer/Shader.h"

#include <glad/glad.h>

namespace Fuze {

class FUZE_API OpenGLShader : public Shader {
  public:
    OpenGLShader(const std::string& filepath);
    OpenGLShader(const std::string& vertexSrc, const std::string& fragmentSrc);
    virtual ~OpenGLShader();

    virtual void Bind() const override;
    virtual void Unbind() const override;

    inline uint32_t GetRendererID() const override {
        return m_RendererID;
    }

    virtual void SetInt(const std::string& name, int value) override;
    virtual void SetFloat4(const std::string& name, const glm::vec4& value) override;
    virtual void SetMat4(const std::string& name, const glm::mat4& value) override;

    void UploadUniformInt(const std::string& name, int value);

    void UploadUniformFloat(const std::string& name, float value);
    void UploadUniformFloat2(const std::string& name, const glm::vec2& value);
    void UploadUniformFloat3(const std::string& name, const glm::vec3& value);
    void UploadUniformFloat4(const std::string& name, const glm::vec4& value);

    void UploadUniformMat3(const std::string& name, const glm::mat3& value);
    void UploadUniformMat4(const std::string& name, const glm::mat4& value);

  private:
    const std::pair<std::string, std::string> ParseShader(const std::string& source);
    const std::string ReadFile(const std::string& filepath);

    void Compile(const std::string& vertexSrc, const std::string& fragmentSrc);

  private:
    uint32_t m_RendererID;
};
}
