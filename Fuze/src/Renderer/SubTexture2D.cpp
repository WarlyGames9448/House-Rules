#include "fuzepch.h"

#include "Renderer/SubTexture2D.h"

namespace Fuze {
SubTexture2D::SubTexture2D(const Ref<Texture2D>& texture, const glm::vec2 min, const glm::vec2 max): m_Texture(texture) {
    float width = texture->GetWidth();
    float height = texture->GetHeight();
    m_TexCoords[0] = {min.x / width, min.y / height};
    m_TexCoords[1] = {max.x / width, min.y / height};
    m_TexCoords[2] = {max.x / width, max.y / height};
    m_TexCoords[3] = {min.x / width, max.y / height};
}

Ref<SubTexture2D> SubTexture2D::Create(const Ref<Texture2D>& texture, const glm::vec2 position, const glm::vec2 spriteSize,
                                       const glm::vec2 tileSize) {
    glm::vec2 min = {position.x * spriteSize.x, position.y * spriteSize.y};
    glm::vec2 max = {((position.x + tileSize.x) * spriteSize.x), (position.y + tileSize.y) * spriteSize.y};

    return CreateRef<SubTexture2D>(texture, min, max);
}

}
