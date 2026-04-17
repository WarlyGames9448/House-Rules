#pragma once

#include "Renderer/Texture.h"

namespace Fuze {
class OpenGLTexture2D : public Texture2D {
  public:
    OpenGLTexture2D(const std::string& path);
    ~OpenGLTexture2D();

    virtual void Bind(uint32_t slot) const override;
    virtual void Unbind() const override;

    virtual int GetHight() const override {
        return m_Height;
    }
    virtual int GetWidth() const override {
        return m_Width;
    }

    virtual int GetChannels() const { return m_Channels;}

  private:
    int m_Width, m_Height, m_Channels;

    unsigned int m_RendererID;
};
}
