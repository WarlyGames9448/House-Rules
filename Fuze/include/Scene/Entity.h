#pragma once

#include "Core.h"
#include <queue>
#include <cstdint>
#include <array>
#include <bitset>

namespace Fuze {

using Entity = std::uint32_t;
const Entity MAX_ENTITIES = 10000;
#define NULL_ENTITY UINT32_MAX

using ComponentType = std::uint8_t;
const ComponentType MAX_COMPONENTS = 32;

using Signature = std::bitset<MAX_COMPONENTS>;

class EntityManager {
  public:
    EntityManager();
    ~EntityManager() = default;

    Entity CreateEntity();
    void DestroyEntity(Entity entity);

    void SetSignature(Entity entity, Signature signature);
    Signature GetSignature(Entity entity);

  private:
    std::queue<Entity> m_FreeIDs;
    Entity m_EntitiesCount = 0;

    std::array<Signature, MAX_ENTITIES> m_Signatures{};

};
}
