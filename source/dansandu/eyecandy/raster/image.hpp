#pragma once

#include "dansandu/eyecandy/raster/color.hpp"

#include <iostream>
#include <stdexcept>
#include <vector>

namespace dansandu {
namespace eyecandy {
namespace raster {

class Image {
    friend bool operator==(const Image& lhs, const Image& rhs) {
        return lhs.width_ == rhs.width_ && lhs.height_ == rhs.height_ &&
               std::equal(lhs.pixels_.begin(), lhs.pixels_.end(), rhs.pixels_.begin(), rhs.pixels_.end());
    }

public:
    using size_type = int;

    Image() : width_{0}, height_{0} {}

    Image(size_type width, size_type height, Color fill = Color{})
        : width_{width}, height_{height}, pixels_(width * height, fill) {
        if (width_ < 0 || height_ < 0 || (width_ != 0) != (height_ != 0))
            throw std::runtime_error("matrix size must be greater or equal than zero");
    }

    Image(const Image&) = default;

    Image(Image&& other) noexcept : width_{other.width_}, height_{other.height_}, pixels_(std::move(other.pixels_)) {
        other.width_ = other.height_ = 0;
    }

    Image& operator=(const Image&) = default;

    Image& operator=(Image&& other) noexcept {
        width_ = other.width_;
        height_ = other.height_;
        pixels_ = std::move(pixels_);
        other.width_ = other.height_ = 0;
        return *this;
    }

    void plot(size_type x, size_type y, Color color) { pixels_[index(x, y)] = color; }

    Color color(size_type x, size_type y) const { return pixels_[index(x, y)]; }

    size_type width() const noexcept { return width_; }

    size_type height() const noexcept { return height_; }

    const uint8_t* pixelArray() const noexcept { return reinterpret_cast<const uint8_t*>(&pixels_[0]); }

private:
    size_type index(size_type x, size_type y) const {
        if (x < 0 || x >= width_ || y < 0 || y >= height_)
            throw std::out_of_range("coordinates exceed image bounds");

        return x + (height_ - y - 1) * width_;
    }

    size_type width_;
    size_type height_;
    std::vector<Color> pixels_;
};

inline bool operator!=(const Image& lhs, const Image& rhs) { return !(lhs == rhs); }
}
}
}
