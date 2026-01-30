#pragma once

#include "Core.h"
#include "Window.h"
#include <GLFW/glfw3.h>

namespace Fuze {
    class FUZE_API LinuxWindow : public Window {
      public:
        LinuxWindow(const WindowProps& windowProps);
        virtual ~LinuxWindow();

        inline unsigned int GetWidth() const override { return m_Data.Width; }
        inline unsigned int GetHeight() const override { return m_Data.Height; }

        void OnUpdate() override;

        void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }
        virtual void SetVSync(bool enabled) override;
        virtual bool IsVSync() const override { return m_Data.VSync; }

      private:
        virtual void Init(const WindowProps& windowProps);
        virtual void Shutdown();

      private:
        GLFWwindow* m_Window;

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