
#pragma once

#include <array>
#include <cstdint>
#include <string>
#include <optional>

namespace Fuze {

class UUID {
  public:
    UUID();

    explicit UUID(const std::string& uuidString);

    static std::optional<UUID> FromString(const std::string& uuidString);

    std::string ToString() const;

    bool IsValid() const;

    bool operator==(const UUID& other) const noexcept;
    bool operator!=(const UUID& other) const noexcept;

  private:
    explicit UUID(const std::array<uint8_t, 16>& data);

    std::array<uint8_t, 16> m_Data {};

    friend struct std::hash<Fuze::UUID>;
};

}

namespace std {

template <> struct hash<Fuze::UUID> {
    size_t operator()(const Fuze::UUID& uuid) const noexcept;
};

}
