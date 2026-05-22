#include "fuzepch.h"
#include "Plataform/OpenGL/OpenGLRendererAPI.h"

#include <glad/glad.h>

namespace Fuze {

void OpenGLRendererAPI::SetBlendMode(BlendMode mode) {
    if (mode == FUZE_BLEND_DEFAULT) {
        glEnable(GL_BLEND);
        glBlendFunc(GL_ONE, GL_ZERO);
    } else if (mode == FUZE_BLEND_ALPHA) {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }
}

void OpenGLRendererAPI::SetClearColor(const glm::vec4& color) {
    FUZE_PROFILE_FUNCTION();

    glClearColor(color.r, color.g, color.b, color.a);
}

void OpenGLRendererAPI::Clear() {
    FUZE_PROFILE_FUNCTION();

    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
}

void OpenGLRendererAPI::DrawIndexed(const Ref<VertexArray>& vertexArray) {
    FUZE_PROFILE_FUNCTION();

    glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
}

void OpenGLRendererAPI::DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount) {
    FUZE_PROFILE_FUNCTION();

    uint32_t count = (indexCount == 0) ? vertexArray->GetIndexBuffer()->GetCount() : indexCount;
    glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
}

void OpenGLRendererAPI::InitializeRenderCaps(){
    m_Caps.GraphicsAPI = "OpenGL";

    // Safe string casting
    const GLubyte* vendor = glGetString(GL_VENDOR);
    const GLubyte* renderer = glGetString(GL_RENDERER);
    m_Caps.Vendor = vendor ? reinterpret_cast<const char*>(vendor) : "Unknown";
    m_Caps.Renderer = renderer ? reinterpret_cast<const char*>(renderer) : "Unknown";

    // Query integers directly into the struct
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, reinterpret_cast<GLint*>(&m_Caps.MaxTextureSize));
    glGetIntegerv(GL_MAX_ARRAY_TEXTURE_LAYERS, reinterpret_cast<GLint*>(&m_Caps.MaxTextureArrayLayers));
    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, reinterpret_cast<GLint*>(&m_Caps.MaxTextureImageUnits));
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, reinterpret_cast<GLint*>(&m_Caps.MaxVertexAttributes));
    glGetIntegerv(GL_MAX_UNIFORM_BLOCK_SIZE, reinterpret_cast<GLint*>(&m_Caps.MaxUniformBlockSize));
    glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, reinterpret_cast<GLint*>(&m_Caps.MaxColorAttachments));

    m_CapsInitialized = true;
}

}
