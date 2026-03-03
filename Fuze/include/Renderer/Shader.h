#pragma once

#include <string>

namespace Fuze {

    class FUZE_API Shader {
      public:
        Shader(const std::string& vertexSrc, const std::string& fragmentSrc);
        ~Shader();

        void Bind();
        void Unbind();

      private:
        uint32_t m_RendererID;
    };
}
