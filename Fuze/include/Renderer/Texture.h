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
    static Texture2D* Create(const std::string& path);

    virtual void Bind(uint32_t slot) const = 0;
    virtual void Unbind() const = 0;
};
}
