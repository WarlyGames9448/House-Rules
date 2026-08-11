#pragma once

#include "Renderer/Framebuffer.h"

namespace Fuze {

class OpenGLFramebuffer : public Framebuffer {
  public:
    OpenGLFramebuffer(const FramebufferSpecification& spec);
    ~OpenGLFramebuffer();

    virtual void Invalidate() override;
    virtual void Bind() const override;
    virtual void Unbind() const override;

    virtual void Resize(uint32_t width, uint32_t height) override;

    uint32_t GetColorAttachmentID() const override{
        return m_ColorAttachment;
    }
    const FramebufferSpecification& GetSpecification() const override{
        return m_Specification;
    }

  private:
    uint32_t m_RendererID = 0;
    uint32_t m_ColorAttachment = 0;
    uint32_t m_DepthAttachment = 0;
    FramebufferSpecification m_Specification;
};
}
