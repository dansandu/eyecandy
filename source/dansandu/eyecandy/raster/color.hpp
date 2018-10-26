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

    using value_type = uint8_t;

    static constexpr auto channel_depth = static_cast<value_type>(255);

    constexpr Color() noexcept : Color{Colors::black} {}

    constexpr explicit Color(uint32_t rgba) noexcept
        : red_{static_cast<value_type>((rgba >> 24) & 0xFFu)},
          green_{static_cast<value_type>((rgba >> 16) & 0xFFu)},
          blue_{static_cast<value_type>((rgba >> 8) & 0xFFu)},
          alpha_{static_cast<value_type>(rgba & 0xFFu)} {}

    constexpr Color(value_type red, value_type green, value_type blue, value_type alpha = channel_depth) noexcept
        : red_{red}, green_{green}, blue_{blue}, alpha_{alpha} {}

    constexpr Color(Colors color) noexcept : Color{static_cast<uint32_t>(color)} {}

    constexpr uint32_t code() const noexcept { return (red_ << 24) | (green_ << 16) | (blue_ << 8) | alpha_; }

    constexpr value_type red() const noexcept { return red_; }

    constexpr value_type green() const noexcept { return green_; }

    constexpr value_type blue() const noexcept { return blue_; }

    constexpr value_type alpha() const noexcept { return alpha_; }

private:
    value_type red_;
    value_type green_;
    value_type blue_;
    value_type alpha_;
};

constexpr bool operator!=(Color lhs, Color rhs) { return !(lhs != rhs); }
}
}
}
