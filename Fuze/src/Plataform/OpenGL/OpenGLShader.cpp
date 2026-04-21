#include "fuzepch.h"
#include "Plataform/OpenGL/OpenGLShader.h"

#include <glad/glad.h>

#include <glm/gtc/type_ptr.hpp>

#include <fstream>

namespace Fuze {

OpenGLShader::OpenGLShader(const std::string& name, const std::string& filepath): m_Name(name) {
    const std::pair<std::string, std::string> shaderSrc = ParseShader(ReadFile(filepath));
    Compile(shaderSrc.first, shaderSrc.second);
}

OpenGLShader::OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
    : m_Name(name) {
    Compile(vertexSrc, fragmentSrc);
}

OpenGLShader::~OpenGLShader() {
    glDeleteProgram(m_RendererID);
}

void OpenGLShader::Compile(const std::string& vertexSrc, const std::string& fragmentSrc) {
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    for (auto& [shader, shaderSrc] : {std::tie(vertexShader, vertexSrc), std::tie(fragmentShader, fragmentSrc)}) {
        const GLchar* source = (const GLchar*)shaderSrc.c_str();
        glShaderSource(shader, 1, &source, 0);

        glCompileShader(shader);

        GLint isCompiled = 0;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
        // Error Handler
        if (isCompiled == GL_FALSE) {
            GLint maxLength = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

            std::vector<GLchar> infoLog(maxLength);
            glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

            glDeleteShader(vertexShader);
            glDeleteShader(fragmentShader);

            FUZE_CORE_ERROR("{0}", infoLog.data());
            FUZE_CORE_ASSERT(false, "Shader failure!");
            return;
        }
    }

    GLuint program = glCreateProgram();

    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);

    glLinkProgram(program);

    GLint isLinked = 0;
    glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
    // Error Handled
    if (isLinked == GL_FALSE) {
        GLint maxLength = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

        std::vector<GLchar> infoLog(maxLength);
        glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

        glDeleteProgram(program);
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        FUZE_CORE_ERROR("{0}", infoLog.data());
        FUZE_CORE_ASSERT(false, "Shader link failure!");
        return;
    }
    m_RendererID = program;

    glDetachShader(program, vertexShader);
    glDetachShader(program, fragmentShader);
}

void OpenGLShader::Bind() const {
    glUseProgram(m_RendererID);
}

void OpenGLShader::Unbind() const {
    glUseProgram(0);
}

const std::pair<std::string, std::string> OpenGLShader::ParseShader(const std::string& source) {

    std::string shaderSrc[2];
    enum Type { VERTEX = 0, FRAGMENT = 1 };
    Type type;

    size_t pos = source.find("#define");

    while (pos != std::string::npos) {
        pos++;

        pos += 7;
        size_t next_pos = source.find_first_of("\n", pos);
        const std::string defineValue = source.substr(pos, next_pos - pos).c_str();
        if (defineValue == "_TYPE_VERTEX_SHADER") type = VERTEX;
        else if (defineValue == "_TYPE_FRAGMENT_SHADER") type = FRAGMENT;
        else FUZE_CORE_ASSERT(0, "Wrong #define syntax: {0}", defineValue);

        pos = source.find("#define", next_pos);

        shaderSrc[type] = std::string(source.substr(next_pos, pos - next_pos - 1));
    }

    std::pair<std::string, std::string> finalSource(shaderSrc[0], shaderSrc[1]);

    return finalSource;
}

// TODO: Move this to file handler system
const std::string OpenGLShader::ReadFile(const std::string& filepath) {
    std::ifstream file(filepath, std::ios::binary | std::ios::ate);

    FUZE_CORE_ASSERT(file.is_open(), "Could not open file: {0}", filepath);

    std::streamsize size = file.tellg();
    if (size == -1) {
        FUZE_CORE_ASSERT(0, "Failed to determine file size: {0}", filepath);
        return "";
    }

    if (size == 0) {
        return "";
    }

    file.seekg(0, std::ios::beg);

    std::string buffer;
    buffer.resize(size);

    if (!file.read(buffer.data(), size)) {
        FUZE_CORE_ASSERT(0, "Could not read the file: {0}", filepath);
    }

    return buffer;
}

void OpenGLShader::UploadUniformInt(const std::string& name, int value) {
    int location = glGetUniformLocation(m_RendererID, name.c_str());
    glUniform1d(location, value);
}

void OpenGLShader::UploadUniformFloat(const std::string& name, float value) {
    int location = glGetUniformLocation(m_RendererID, name.c_str());
    glUniform1d(location, value);
}

void OpenGLShader::UploadUniformFloat2(const std::string& name, const glm::vec2& value) {
    int location = glGetUniformLocation(m_RendererID, name.c_str());
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
}

void OpenGLShader::UploadUniformFloat3(const std::string& name, const glm::vec3& value) {
    int location = glGetUniformLocation(m_RendererID, name.c_str());
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
}

void OpenGLShader::UploadUniformFloat4(const std::string& name, const glm::vec4& value) {
    int location = glGetUniformLocation(m_RendererID, name.c_str());
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
}

void OpenGLShader::UploadUniformMat3(const std::string& name, const glm::mat3& value) {
    int location = glGetUniformLocation(m_RendererID, name.c_str());
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
}

void OpenGLShader::UploadUniformMat4(const std::string& name, const glm::mat4& value) {
    int location = glGetUniformLocation(m_RendererID, name.c_str());
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
}
}
