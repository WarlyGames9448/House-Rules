#pragma once

#include "Core.h"

namespace Fuze {

struct FramebufferSpecification {
    uint32_t Width = 0;
    uint32_t Height = 0;
    // TODO: texture format, MSAA, swap chain target.
};

class FUZE_API Framebuffer {
  public:
    static Ref<Framebuffer> Create(const FramebufferSpecification& spec);
    virtual ~Framebuffer() = default;

    virtual void Invalidate() = 0;
    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;

    virtual void Resize(uint32_t width, uint32_t height) = 0;

    virtual uint32_t GetColorAttachmentID() const = 0;

    virtual const FramebufferSpecification& GetSpecification() const = 0;
};
}
