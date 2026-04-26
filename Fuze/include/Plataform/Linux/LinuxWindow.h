#pragma once

#include "Core.h"
#include "Window.h"
#include <GLFW/glfw3.h>
#include "Renderer/GraphicsContext.h"

namespace Fuze {
class LinuxWindow : public Window {
  public:
    LinuxWindow(const WindowProps& windowProps);
    virtual ~LinuxWindow();

    inline unsigned int GetWidth() const override { return m_Data.Width; }
    inline unsigned int GetHeight() const override { return m_Data.Height; }

    void OnUpdate() override;

    void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }
    virtual void SetVSync(bool enabled) override;
    virtual bool IsVSync() const override { return m_Data.VSync; }

    virtual void* GetNativeWindow() const override { return m_Window; }

    virtual Timestep GetTime() const override;

    virtual void SetTitle(const std::string& title) override;
    virtual void SetIcon(const std::string& filepath) override;

  private:
    virtual void Init(const WindowProps& windowProps);
    virtual void Shutdown();

  private:
    GLFWwindow* m_Window;
    Scope<GraphicsContext> m_Context;

    struct WindowData {
        std::string Title;
        unsigned int Width;
        unsigned int Height;
        bool VSync;

        EventCallbackFn EventCallback;
    };

    WindowData m_Data;
};
}
