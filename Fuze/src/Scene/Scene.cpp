#include "fuzepch.h"

#include "Scene/Scene.h"
#include "Renderer/Renderer2D.h"

namespace Fuze {
// RenderSystem ============================
void RenderSystem::OnUpdate(Timestep ts [[maybe_unused]]) {
    for (auto& entity : m_Entities) {
        auto transform = m_Registry->GetComponent<TransformComponent>(entity);
        auto sprite = m_Registry->GetComponent<SpriteRendererComponent>(entity);

        Renderer2D::DrawQuad(transform.Transform, sprite.Color);
    }
}

// Scene ====================================
Scene::Scene() {
    m_Registry = CreateRef<Registry>();

    m_Registry->RegisterComponent<SpriteRendererComponent>();
    m_Registry->RegisterComponent<TransformComponent>();

    m_RenderSystem = m_Registry->RegisterSystem<RenderSystem>();

    Signature sceneSignature;
    sceneSignature.set(m_Registry->GetComponentType<SpriteRendererComponent>());
    sceneSignature.set(m_Registry->GetComponentType<TransformComponent>());
    m_Registry->SetSystemSignature<RenderSystem>(sceneSignature);
}

Scene::~Scene() {
}

void Scene::OnUpdate(Timestep ts [[maybe_unused]]) {
    m_RenderSystem->OnUpdate(ts);
}
}
