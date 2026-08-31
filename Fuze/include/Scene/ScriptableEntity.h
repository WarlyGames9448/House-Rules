#pragma once

#include "Scene/Entity.h"
#include "Scene/Registry.h"

#include "Timestep.h"

namespace Fuze {

class Registry;

class ScriptableEntity {
  public:
    virtual ~ScriptableEntity() = default;

    virtual void OnCreate() {
    }
    virtual void OnUpdate(Timestep ts) {
    }
    virtual void OnDestroy() {
    }

  protected:
    template <typename T> T& GetComponent() {
        return m_Registry->GetComponent<T>(m_Entity);
    }

    template <typename T> bool HasComponent() {
        return m_Registry->HasComponent<T>(m_Entity);
    }

    template <typename T> void AddComponent(T component) {
        m_Registry->AddComponent<T>(m_Entity, component);
    }

    template <typename T> T& GetComponent(Entity targetEntity) {
        return m_Registry->GetComponent<T>(targetEntity);
    }

    template <typename T> bool HasComponent(Entity targetEntity) {
        return m_Registry->HasComponent<T>(targetEntity);
    }

    Entity GetID() const {
        return m_Entity;
    }

  private:
    Entity m_Entity;
    Registry* m_Registry = nullptr;

    friend class NativeScriptSystem;
};

struct NativeScriptComponent {
    ScriptableEntity* Instance = nullptr;

    ScriptableEntity* (*InstantiateScript)() = nullptr;
    void (*DestroyScript)(NativeScriptComponent*) = nullptr;

    template <typename T> void Bind() {
        InstantiateScript = []() -> ScriptableEntity* { return static_cast<ScriptableEntity*>(new T()); };

        DestroyScript = [](NativeScriptComponent* nsc) {
            delete nsc->Instance;
            nsc->Instance = nullptr;
        };
    }
};

class NativeScriptSystem : public System {
  public:
    void OnUpdate(Timestep ts) override {
        for (const auto& entity : m_Entities) {
            auto& script = m_Registry->GetComponent<NativeScriptComponent>(entity);

            // Lazy Instantiation
            if (!script.Instance) {
                script.Instance = script.InstantiateScript();

                // NativeScriptSystem is a friend class
                script.Instance->m_Entity = entity;
                script.Instance->m_Registry = m_Registry;

                script.Instance->OnCreate();
            }

            script.Instance->OnUpdate(ts);
        }
    }

    // TEMP
    // TODO: Intercept event EntityDestroyed and call OnDestroy();
    void Cleanup() {
        for (const auto& entity : m_Entities) {
            auto& script = m_Registry->GetComponent<NativeScriptComponent>(entity);
            if (script.Instance) {
                script.Instance->OnDestroy();
                script.DestroyScript(&script);
            }
        }
    }
};
}
