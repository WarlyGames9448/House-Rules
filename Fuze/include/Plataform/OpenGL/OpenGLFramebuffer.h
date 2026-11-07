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

    uint32_t GetColorAttachmentID(uint32_t index = 0) const override {
        FUZE_CORE_ASSERT(index < m_ColorAttachments.size());
        return m_ColorAttachments[index];
    }
    const FramebufferSpecification& GetSpecification() const override {
        return m_Specification;
    }

  private:
    uint32_t m_RendererID = 0;
    FramebufferSpecification m_Specification;

    std::vector<FramebufferTextureSpecification> m_ColorAttachmentSpecifications;
    FramebufferTextureSpecification m_DepthAttachmentSpecification = FramebufferTextureFormat::None;

    std::vector<uint32_t> m_ColorAttachments;
    uint32_t m_DepthAttachment = 0;
};
}
