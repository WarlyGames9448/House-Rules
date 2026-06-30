#pragma once

#include "Fuze.h"

namespace Fuze {
static const int MAXPARTICLES = 50000;

struct ParticleList {
    // Going to GPU
    glm::vec2 Position[MAXPARTICLES];
    glm::vec4 Color[MAXPARTICLES];

    glm::vec2 Velocity[MAXPARTICLES];
    glm::vec2 Acceleration[MAXPARTICLES];

    float LifeRemaining[MAXPARTICLES];
};

class ParticleSystem {
  private:
    ParticleList m_Particles;
    int m_AliveCount;

    Ref<VertexBuffer> m_PosVBO;
    Ref<VertexBuffer> m_ColorVBO;
    Ref<VertexArray> m_VertexArray;
    Ref<IndexBuffer> m_IndexBuffer;
    Ref<Shader> m_Shader;

  public:
    ParticleSystem();

    void OnUpdate(Timestep ts);
    void OnRender(Ref<OrthographicCamera> camera);

    bool AddParticle(const glm::vec2 position, const glm::vec2 velocity, const glm::vec2 acceleration, const glm::vec4 color, float lifeRemaining);

  private:
    void CopyParticle(int from, int to);
};

}
