#pragma once

#include "Scene/Component.h"

#include <set>

namespace Fuze {

class Registry;

class System {
  public:
    void SetRegistry(Registry* registry) {
        m_Registry = registry;
    }

    virtual void Init() {
    }
    virtual void Update(float dt) {
    }

    // TODO:
    //  virtual void Render() {}

  public:
    // TODO: change set to contiguos array
    std::set<Entity> Entities;

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

        Ref<System> system = CreateRef<T>();
        m_Systems.insert({typeName, system});
        return std::static_pointer_cast<T>(system);
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
            system->Entities.erase(entity);
        }
    }

    void EntitySignatureChanged(Entity entity, Signature entitySignature) {
        for (auto& [typeName, system] : m_Systems) {
            Signature systemSignature = m_Signatures[typeName];
            if ((entitySignature & systemSignature) == systemSignature) {
                system->Entities.insert(entity);
            } else {
                system->Entities.erase(entity);
            }
        }
    }

  private:
    std::unordered_map<const char*, Signature> m_Signatures;
    std::unordered_map<const char*, Ref<System>> m_Systems;
};
}
