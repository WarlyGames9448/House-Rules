#include "Core.h"
#include "fuzepch.h"

#include "Plataform/OpenGL/OpenGLTexture.h"

#include <stb_image.h>
#include <glad/glad.h>

namespace Fuze {
OpenGLTexture2D::OpenGLTexture2D(): m_Width(1), m_Height(1) {
    FUZE_PROFILE_FUNCTION();

    unsigned char whitePixel[] = { 255, 255, 255, 255 };

    glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);

    glTextureStorage2D(m_RendererID, 1, GL_RGBA8, 1, 1);

    glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glTextureSubImage2D(m_RendererID, 0, 0, 0, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, whitePixel);
}

OpenGLTexture2D::OpenGLTexture2D(const std::string& path) {
    FUZE_PROFILE_FUNCTION();

    stbi_set_flip_vertically_on_load(true);

    stbi_uc* data = stbi_load(path.c_str(), &m_Width, &m_Height, &m_Channels, 0);
    if (!data) FUZE_CORE_ERROR("Failed to load texture! Reason: {0}", stbi_failure_reason());

    GLenum internalFormat = 0, dataFormat = 0;
    if (m_Channels == 4) {
        internalFormat = GL_RGBA8;
        dataFormat = GL_RGBA;
    } else if (m_Channels == 3) {
        internalFormat = GL_RGB8;
        dataFormat = GL_RGB;
    }
    FUZE_CORE_ASSERT(internalFormat | dataFormat, "Format not suported!")

    glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);

    glTextureStorage2D(m_RendererID, 1, internalFormat, m_Width, m_Height);

    glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, dataFormat, GL_UNSIGNED_BYTE, data);

    // glGenerateTextureMipmap(m_RendererID);

    stbi_image_free(data);
}

OpenGLTexture2D::~OpenGLTexture2D() {
    FUZE_PROFILE_FUNCTION();

    glDeleteTextures(1, &m_RendererID);
}

void OpenGLTexture2D::Bind(uint32_t slot) const {
    FUZE_PROFILE_FUNCTION();

    glBindTextureUnit(slot, m_RendererID);
}

void OpenGLTexture2D::Unbind(uint32_t slot) const {
    FUZE_PROFILE_FUNCTION();

    glBindTextureUnit(slot, 0);
}
}
