#pragma once

namespace Fuze {
class FUZE_API Texture {
  public:
    virtual ~Texture() = default;

    virtual int GetHight() const = 0;
    virtual int GetWidth() const = 0;
};

class FUZE_API Texture2D : public Texture {
  public:
    static Ref<Texture2D> Create(const std::string& path);

    virtual void Bind(uint32_t slot = 0) const = 0;
    virtual void Unbind(uint32_t slot = 0) const = 0;
};
}
