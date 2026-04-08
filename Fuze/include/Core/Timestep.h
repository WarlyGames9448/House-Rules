#pragma once

namespace Fuze {
class FUZE_API Timestep {
  public:
    Timestep(float time): m_time(time) {}

    operator float() { return m_time; }

    inline float GetSeconds() const { return m_time; }
    inline float GetMiliseconds() const { return m_time * 1000; }

  private:
    float m_time;
};
}
