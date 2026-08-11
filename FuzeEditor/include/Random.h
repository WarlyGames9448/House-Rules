#pragma once

#include <random>
#include <type_traits>

namespace Fuze {
namespace Random {
// Thread-local Engine for lock-free multi-threading efficiency
inline std::mt19937_64& GetEngine() {
    // random_device is used once per thread to seed the engine
    thread_local std::random_device rd;

    // mt19937_64 is a fast, 64-bit Mersenne Twister engine
    thread_local std::mt19937_64 engine(rd());

    return engine;
}

// Uniform Integer Distribution [min, max]
template <typename T> inline T GetInt(T min, T max) {
    FUZE_ASSERT(std::is_integral_v<T>, "T must be an integral type");
    std::uniform_int_distribution<T> dist(min, max);
    return dist(GetEngine());
}

// Uniform Floating-Point Distribution [min, max)
template <typename T> inline T GetFloat(T min, T max) {
    FUZE_ASSERT(std::is_floating_point_v<T>, "T must be a floating point type");
    std::uniform_real_distribution<T> dist(min, max);
    return dist(GetEngine());
}

// Bernoulli Distribution (True/False based on probability)
inline bool GetBool(double true_probability = 0.5) {
    std::bernoulli_distribution dist(true_probability);
    return dist(GetEngine());
}

// Normal Distribution (Gaussian)
template <typename T> inline T GetNormal(T mean, T std_dev) {
    FUZE_ASSERT(std::is_floating_point_v<T>, "T must be a floating point type");
    std::normal_distribution<T> dist(mean, std_dev);
    return dist(GetEngine());
}
}
}
