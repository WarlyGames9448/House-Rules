#pragma once

#include "Scene/Component.h"
#include "Core/Timestep.h"

#include <set>

namespace Fuze {

class Registry;

class System {
  public:
    virtual ~System() = default;

    void SetRegistry(Registry* registry) {
        m_Registry = registry;
    }

    virtual void Init() {
    }
    virtual void OnUpdate([[maybe_unused]] Timestep ts) {
    }

    // TEMP
    virtual Registry* Reg() {
        return m_Registry;
    }

    // TODO:
    //  virtual void Render() {}

  public:
    // TODO: change set to contiguos array
    std::set<Entity> m_Entities;

  protected:
    Registry* m_Registry;
};

class SystemManager {
  public:
    template <typename T> Ref<T> RegisterSystem() {
        const char* typeName = typeid(T).name();

        if (m_Systems.contains(typeName)) {
            FUZE_CORE_ASSERT(0, "SystemManager Already has this System.");
            return nullptr;
        }

        auto system = CreateRef<T>();
        m_Systems.insert({typeName, system});
        return system;
    }

    template <typename T> void SetSignature(Signature signature) {
        const char* typeName = typeid(T).name();

        if (!m_Systems.contains(typeName)) {
            FUZE_CORE_ASSERT(0, "SystemManager does not have this System.");
            return;
        }

        m_Signatures.insert({typeName, signature});
    }

    void EntityDestroyed(Entity entity) {
        for (auto& [typeName, system] : m_Systems) {
            system->m_Entities.erase(entity);
        }
    }

    void EntitySignatureChanged(Entity entity, Signature entitySignature) {
        for (auto& [typeName, system] : m_Systems) {
            Signature systemSignature = m_Signatures[typeName];
            if ((entitySignature & systemSignature) == systemSignature) {
                system->m_Entities.insert(entity);
            } else {
                system->m_Entities.erase(entity);
            }
        }
    }

  private:
    std::unordered_map<const char*, Signature> m_Signatures;
    std::unordered_map<const char*, Ref<System>> m_Systems;
};
}
