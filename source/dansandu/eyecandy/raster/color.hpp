#pragma once

#include "dansandu/eyecandy/raster/colors.hpp"

#include <cstdint>

namespace dansandu {
namespace eyecandy {
namespace raster {

class Color {
public:
    friend constexpr bool operator==(Color lhs, Color rhs) {
        return lhs.red_ == rhs.red_ && lhs.green_ == rhs.green_ && lhs.blue_ == rhs.blue_ && lhs.alpha_ == rhs.alpha_;
    }

    constexpr Color() noexcept : Color{Colors::black} {}

    constexpr explicit Color(uint32_t rgba) noexcept
        : red_{static_cast<uint8_t>((rgba >> 24) & 0xFFu)},
          green_{static_cast<uint8_t>((rgba >> 16) & 0xFFu)},
          blue_{static_cast<uint8_t>((rgba >> 8) & 0xFFu)},
          alpha_{static_cast<uint8_t>(rgba & 0xFFu)} {}

    constexpr Color(Colors color) noexcept : Color{static_cast<uint32_t>(color)} {}

    constexpr uint32_t code() const noexcept { return (red_ << 24) | (green_ << 16) | (blue_ << 8) | alpha_; }

private:
    uint8_t red_;
    uint8_t green_;
    uint8_t blue_;
    uint8_t alpha_;
};

constexpr bool operator!=(Color lhs, Color rhs) { return !(lhs != rhs); }
}
}
}
