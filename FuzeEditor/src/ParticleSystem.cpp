#include "ParticleSystem.h"

#include "Utils/FileUtils.h"

namespace Fuze {
ParticleSystem::ParticleSystem(): m_AliveCount(0) {

    for (int i = 0; i < MAXPARTICLES; i++) {
        m_Particles.Position[i] = {0, 0};
        m_Particles.Color[i] = {0, 0, 0, 0};
        m_Particles.Velocity[i] = {0, 0};
        m_Particles.Acceleration[i] = {0, 0};
        m_Particles.LifeRemaining[i] = 0;
    }
    // Static VBO (single quad)
    // clang-format off
    float particleVertices[] = {
        -0.05f,  0.05f,
        -0.05f, -0.05f,
         0.05f, -0.05f,
         0.05f,  0.05f,
    };

    uint32_t particleIndices[] = {
        0, 1, 2, 2, 3, 0
    };
    // clang-format on

    Ref<VertexBuffer> BaseVertexBuffer = VertexBuffer::Create(particleVertices, sizeof(particleVertices));
    BaseVertexBuffer->SetLayout({
        {ShaderDataType::Float2, "a_Position"}
    });

    m_PosVBO = VertexBuffer::Create(MAXPARTICLES * sizeof(glm::vec2));
    m_PosVBO->SetLayout({
        {ShaderDataType::Float2, "a_InstanceOffset", 1},
    });

    m_ColorVBO = VertexBuffer::Create(MAXPARTICLES * sizeof(glm::vec4));
    m_ColorVBO->SetLayout({
        {ShaderDataType::Float4, "a_InstanceColor", 1}
    });

    m_VertexArray = VertexArray::Create();
    m_VertexArray->AddVertexBuffer(BaseVertexBuffer);
    m_VertexArray->AddVertexBuffer(m_PosVBO);
    m_VertexArray->AddVertexBuffer(m_ColorVBO);

    m_IndexBuffer = IndexBuffer::Create(particleIndices, 6);
    m_VertexArray->SetIndexBuffer(m_IndexBuffer);

    m_Shader = Shader::Create(FileUtils::GetSandboxAsset("shaders/particle.glsl"));
}

void ParticleSystem::OnUpdate(Timestep ts) {
    FUZE_PROFILE_FUNCTION();
    // Swap-Back deletion: When a particle dies, copy the last alive particle into dead particle
    for (int index = 0; index < m_AliveCount; index++) {
        m_Particles.LifeRemaining[index] -= ts.GetSeconds();
        if (m_Particles.LifeRemaining[index] <= 0) {
            CopyParticle(m_AliveCount - 1, index);
            m_AliveCount--;
            index--;
        } else {
            m_Particles.Position[index] += m_Particles.Velocity[index] * ts.GetSeconds();
            m_Particles.Velocity[index] += m_Particles.Acceleration[index] * ts.GetSeconds();
        }
    }
}

void ParticleSystem::OnRender(Ref<OrthographicCamera> camera) {
    FUZE_PROFILE_FUNCTION();
    m_Shader->Bind();
    m_Shader->SetMat4("u_ViewProjection", camera->GetViewProjection());
    m_PosVBO->SetData(m_Particles.Position, m_AliveCount * sizeof(glm::vec2));
    m_ColorVBO->SetData(m_Particles.Color, m_AliveCount * sizeof(glm::vec4));
    RendererCommand::DrawIndexedInstanced(m_VertexArray, m_AliveCount);
}

void ParticleSystem::CopyParticle(int from, int to) {
    FUZE_PROFILE_FUNCTION();

    m_Particles.Position[to] = m_Particles.Position[from];
    m_Particles.Color[to] = m_Particles.Color[from];

    m_Particles.Velocity[to] = m_Particles.Velocity[from];
    m_Particles.Acceleration[to] = m_Particles.Acceleration[from];

    m_Particles.LifeRemaining[to] = m_Particles.LifeRemaining[from];
}

bool ParticleSystem::AddParticle(const glm::vec2 position, const glm::vec2 velocity, const glm::vec2 acceleration, const glm::vec4 color, float lifeRemaining) {
    FUZE_PROFILE_FUNCTION();

    if (m_AliveCount < MAXPARTICLES) {
        m_Particles.Position[m_AliveCount] = position;
        m_Particles.Color[m_AliveCount] = color;

        m_Particles.Velocity[m_AliveCount] = velocity;
        m_Particles.Acceleration[m_AliveCount] = acceleration;

        m_Particles.LifeRemaining[m_AliveCount] = lifeRemaining;

        m_AliveCount++;
        return true;
    }

    return false;
}
}
