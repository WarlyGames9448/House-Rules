#pragma once

#include "fuzepch.h"
#include "Scene/Entity.h"

namespace Fuze {

class IComponentArray {
  public:
    virtual void EntityDestroyed(Entity entity) = 0;
    ~IComponentArray() = default;
};

template <typename T> class ComponentArray : public IComponentArray {
  public:
    ComponentArray() {
        for (Entity i = 0; i < MAX_ENTITIES; i++) {
            m_EntityToIndexMap[i] = UINT32_MAX;
        }
    }

    void InsertData(Entity entity, T component) {
        if (m_EntityToIndexMap[entity] != UINT32_MAX) {
            FUZE_CORE_ASSERT(0, "Entity {0} already have this component", entity);
            return;
        }
        m_ComponentArray[m_ValidSize] = component;

        m_EntityToIndexMap[entity] = m_ValidSize;
        m_IndexToEntityMap[m_ValidSize] = entity;

        m_ValidSize++;
    }

    void RemoveData(Entity entity) {
        if (m_EntityToIndexMap[entity] == UINT32_MAX) {
            FUZE_CORE_WARN("Entity {0} does not have this component.", entity);
            return;
        }

        size_t indexOfRemovedEntity = m_EntityToIndexMap[entity];
        size_t indexOfLastElement = m_ValidSize - 1;
        m_ComponentArray[indexOfRemovedEntity] = m_ComponentArray[indexOfLastElement];

        Entity lastEntity = m_IndexToEntityMap[indexOfLastElement];
        m_EntityToIndexMap[lastEntity] = indexOfRemovedEntity;
        m_IndexToEntityMap[indexOfRemovedEntity] = lastEntity;

        m_EntityToIndexMap[entity] = UINT32_MAX;

        m_ValidSize--;
    }

    T& GetData(Entity entity) {
        if (m_EntityToIndexMap[entity] == UINT32_MAX) {
            FUZE_CORE_ASSERT(0, "Entity {0} does not have this component.", entity);
        }

        return m_ComponentArray[m_EntityToIndexMap[entity]];
    }

    void EntityDestroyed(Entity entity) override {
        if (m_EntityToIndexMap[entity] != UINT32_MAX) {
            RemoveData(entity);
        }
    }

    size_t GetSize() {
        return m_ValidSize;
    }

  private:
    std::array<T, MAX_ENTITIES> m_ComponentArray;
    std::array<size_t, MAX_ENTITIES> m_EntityToIndexMap;
    std::array<Entity, MAX_ENTITIES> m_IndexToEntityMap;

    size_t m_ValidSize = 0;
};

class ComponentManager {
  public:
    template <typename T> void RegisterComponent() {
        const char* typeName = typeid(T).name();

        if (m_ComponentTypes.contains(typeName)) {
            return;
        }

        m_ComponentTypes.insert({typeName, m_NextComponentType});
        Ref<IComponentArray> componentArray = CreateRef<ComponentArray<T>>();
        m_ComponentArrays.insert({typeName, componentArray});

        m_NextComponentType++;
    }

    template <typename T> ComponentType GetComponentType() {
        const char* typeName = typeid(T).name();

        if (!m_ComponentTypes.contains(typeName)) {
            FUZE_CORE_ASSERT(0, "ComponentManager does not have this component.");
            return UINT8_MAX;
        }

        return m_ComponentTypes[typeName];
    }

    template <typename T> void InsertComponent(Entity entity, T component) {
        GetComponentArray<T>()->InsertData(entity, component);
    }

    template <typename T> void RemoveComponent(Entity entity) {
        GetComponentArray<T>()->RemoveData(entity);
    }

    template <typename T> T& GetComponent(Entity entity) {
        return GetComponentArray<T>()->GetData(entity);
    }

    void EntityDestroyed(Entity entity) {
        for (const auto& [typeName, componentArray] : m_ComponentArrays) {
            componentArray->EntityDestroyed(entity);
        }
    }

  private:
    template <typename T> const Ref<ComponentArray<T>> GetComponentArray() {
        const char* typeName = typeid(T).name();

        if (!m_ComponentTypes.contains(typeName)) {
            FUZE_CORE_ASSERT(0, "ComponentManager does not have this component.");
            return nullptr;
        }

        return std::static_pointer_cast<ComponentArray<T>>(m_ComponentArrays[typeName]);
    }

  private:
    std::unordered_map<const char*, ComponentType> m_ComponentTypes;
    std::unordered_map<const char*, Ref<IComponentArray>> m_ComponentArrays;

    ComponentType m_NextComponentType = 0;
};

/// Components

struct TransformComponent {
    glm::mat4 Transform {1.0f};

    TransformComponent() = default;
    TransformComponent(const TransformComponent&) = default; // Copy
    TransformComponent(const glm::mat4 transform): Transform(transform) {
    }

    operator glm::mat4() {
        return Transform;
    }
    operator const glm::mat4() {
        return Transform;
    }
};

struct SpriteRendererComponent {
    glm::vec4 Color {1.0f, 1.0f, 1.0f, 1.0f};

    SpriteRendererComponent() = default;
    SpriteRendererComponent(const SpriteRendererComponent&) = default; // Copy
    SpriteRendererComponent(const glm::vec4 color): Color(color) {
    }
};

}
