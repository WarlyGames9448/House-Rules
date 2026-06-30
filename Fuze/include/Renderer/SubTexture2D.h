#pragma once

#include "Texture.h"

namespace Fuze {
// The Spritesheet must be tight (no margins)
class FUZE_API SubTexture2D {
  public:
    // Create from exact pixel range
    SubTexture2D(const Ref<Texture2D>& texture, const glm::vec2 min, const glm::vec2 max);

    // Create from tiles
    // Ex. Create(texture, {6,7}, {32, 32}, {1, 2})
    static Ref<SubTexture2D> Create(const Ref<Texture2D>& texture, const glm::vec2 position, const glm::vec2 spriteSize, const glm::vec2 tileSize);

    inline glm::vec2* GetTexCoords() {
        return m_TexCoords;
    }

    inline Ref<Texture2D> GetTexture() {
        return m_Texture;
    }

  private:
    Ref<Texture2D> m_Texture;
    glm::vec2 m_TexCoords[4];
};
}
