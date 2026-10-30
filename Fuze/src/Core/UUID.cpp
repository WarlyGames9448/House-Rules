
#include "fuzepch.h"
#include "UUID.h"

#include <uuid.h>

#include <random>
#include <mutex>
#include <stdexcept>
#include <algorithm>

namespace Fuze {

namespace {

uuids::uuid GenerateUUID() {
    // Shared random engine.
    static std::random_device randomDevice;

    static std::mt19937 engine = [] {
        std::array<int, std::mt19937::state_size> seedData {};

        std::generate(seedData.begin(), seedData.end(), std::ref(randomDevice));

        std::seed_seq seed(seedData.begin(), seedData.end());

        return std::mt19937(seed);
    }();

    static uuids::uuid_random_generator generator(engine);

    // Protect the shared random engine.
    static std::mutex mutex;

    std::lock_guard<std::mutex> lock(mutex);

    return generator();
}

}

UUID::UUID() {
    auto nativeUUID = GenerateUUID();
    auto bytes = nativeUUID.as_bytes();

    std::memcpy(m_Data.data(), bytes.data(), m_Data.size());
}

UUID::UUID(const std::array<uint8_t, 16>& data): m_Data(data) {
}

UUID::UUID(const std::string& uuidString) {
    auto parsedUUID = uuids::uuid::from_string(uuidString);

    if (!parsedUUID.has_value()) throw std::invalid_argument("Invalid UUID string.");

    auto bytes = parsedUUID->as_bytes();

    std::memcpy(m_Data.data(), bytes.data(), m_Data.size());
}

std::optional<UUID> UUID::FromString(const std::string& uuidString) {
    auto parsedUUID = uuids::uuid::from_string(uuidString);

    if (!parsedUUID.has_value()) return std::nullopt;

    auto bytes = parsedUUID->as_bytes();

    std::array<uint8_t, 16> data {};
    std::memcpy(data.data(), bytes.data(), data.size());

    return UUID(data);
}

std::string UUID::ToString() const {
    uuids::uuid nativeUUID(m_Data);

    return uuids::to_string(nativeUUID);
}

bool UUID::IsValid() const {
    return std::any_of(m_Data.begin(), m_Data.end(), [](uint8_t byte) { return byte != 0; });
}

bool UUID::operator==(const UUID& other) const noexcept {
    return m_Data == other.m_Data;
}

bool UUID::operator!=(const UUID& other) const noexcept {
    return !(*this == other);
}

}

namespace std {

size_t hash<Fuze::UUID>::operator()(const Fuze::UUID& uuid) const noexcept {
    size_t result = 0;

    for (uint8_t byte : uuid.m_Data) {
        result ^= std::hash<uint8_t> {}(byte) + 0x9e3779b9 + (result << 6) + (result >> 2);
    }

    return result;
}

}
