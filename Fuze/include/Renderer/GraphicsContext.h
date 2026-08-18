#pragma once

namespace Fuze {

class FUZE_API GraphicsContext {

  public:
    virtual ~GraphicsContext() = default;
    virtual void Init() = 0;
    virtual void SwapBuffers() = 0;
};
}
