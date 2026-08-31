#pragma once

#include "Scene/Entity.h"
#include "Scene/System.h"

namespace Fuze {

class Registry {
  public:
    Registry() {
        m_EntityManager = CreateScope<EntityManager>();
        m_ComponentManager = CreateScope<ComponentManager>();
        m_SystemManager = CreateScope<SystemManager>();
    }
    ~Registry() = default;

    Entity CreateEntity() {
        return m_EntityManager->CreateEntity();
    }

    void DestroyEntity(Entity entity) {
        m_EntityManager->DestroyEntity(entity);
        m_ComponentManager->EntityDestroyed(entity);
        m_SystemManager->EntityDestroyed(entity);
    }

    std::set<Entity> GetEntities() {
        return m_EntityManager->GetEntities();
    }

    template <typename T> Ref<T> RegisterSystem() {
        auto system = m_SystemManager->RegisterSystem<T>();
        system->SetRegistry(this);
        return system;
    }

    template <typename T> void SetSystemSignature(Signature signature) {
        m_SystemManager->SetSignature<T>(signature);
    }

    template <typename T> void RegisterComponent() {
        m_ComponentManager->RegisterComponent<T>();
    }

    template <typename T> T& GetComponent(Entity entity) {
        return m_ComponentManager->GetComponent<T>(entity);
    }

    template <typename T> bool HasComponent(Entity entity) {
        auto entitySignature = m_EntityManager->GetSignature(entity);

        return entitySignature.test(m_ComponentManager->GetComponentType<T>());
    }

    template <typename T> ComponentType GetComponentType() {
        return m_ComponentManager->GetComponentType<T>();
    }

    template <typename T> void AddComponent(Entity entity, T component) {
        m_ComponentManager->InsertComponent<T>(entity, component);

        Signature signature = m_EntityManager->GetSignature(entity);
        signature.set(m_ComponentManager->GetComponentType<T>(), true);
        m_EntityManager->SetSignature(entity, signature);

        m_SystemManager->EntitySignatureChanged(entity, signature);
    }

    template <typename T> void RemoveComponent(Entity entity) {
        m_ComponentManager->RemoveComponent<T>(entity);

        Signature signature = m_EntityManager->GetSignature(entity);
        signature.set(m_ComponentManager->GetComponentType<T>(), false);
        m_EntityManager->SetSignature(entity, signature);

        m_SystemManager->EntitySignatureChanged(entity, signature);
    }

  private:
    Scope<EntityManager> m_EntityManager;
    Scope<ComponentManager> m_ComponentManager;
    Scope<SystemManager> m_SystemManager;
};
}
