#pragma once

namespace Fuze {
    enum class RendererAPI {
        none = 0,
        OpenGL = 1,
    };

    class FUZE_API Renderer {
      public:
        inline static RendererAPI GetAPI() { return s_RendererAPI; }

      private:
        static RendererAPI s_RendererAPI;
    };
}
