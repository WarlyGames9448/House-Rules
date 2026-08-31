#include "fuzepch.h"

#include "Scene/Entity.h"

namespace Fuze {
EntityManager::EntityManager() {
    for (Entity i = 0; i < MAX_ENTITIES; i++) {
        m_FreeIDs.push(i);
    }
}

Entity EntityManager::CreateEntity() {
    if (m_EntitiesCount < MAX_ENTITIES) {
        Entity entity = m_FreeIDs.front();
        m_FreeIDs.pop();
        m_EntitiesCount++;

        m_Entities.insert(entity);

        return entity;
    }

    else {
        FUZE_CORE_ASSERT(0, "Max entities reached!");
        return -1;
    }
}

void EntityManager::DestroyEntity(Entity entity) {
    if (entity >= MAX_ENTITIES) {
        FUZE_CORE_ASSERT(0, "Entity ID is not in scope {0}", entity);
        return;
    }

    m_Signatures[entity].reset();

    m_FreeIDs.push(entity);
    m_EntitiesCount--;

    m_Entities.erase(entity);

}

void EntityManager::SetSignature(Entity entity, Signature signature) {
    if (entity >= MAX_ENTITIES) {
        FUZE_CORE_ASSERT(0, "Entity ID is not in scope {0}", entity);
        return;
    }
    m_Signatures[entity] = signature;
}

Signature EntityManager::GetSignature(Entity entity) {
    if (entity >= MAX_ENTITIES) {
        FUZE_CORE_ASSERT(0, "Entity ID is not in scope {0}", entity);
        return -1;
    }
    return m_Signatures[entity];
}
}
