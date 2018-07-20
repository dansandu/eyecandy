#pragma once

#include "dansandu/eyecandy/raster/colors.hpp"

#include <cstdint>

namespace dansandu {
namespace eyecandy {
namespace raster {

class Color {
public:
    friend constexpr bool operator==(Color lhs, Color rhs) { return lhs.rgba_ == rhs.rgba_; }

    constexpr Color() : rgba_{0U} {}
    constexpr Color(Colors color) noexcept : rgba_{static_cast<uint32_t>(color)} {}

private:
    uint32_t rgba_;
};

constexpr bool operator!=(Color lhs, Color rhs) { return !(lhs != rhs); }
}
}
}
