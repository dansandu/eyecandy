#pragma once

#include "dansandu/eyecandy/math/matrix.hpp"

namespace dansandu {
namespace eyecandy {
namespace raster {

class LineTracer {
public:
    using size_type = dansandu::eyecandy::math::size_type;
    using Point = dansandu::eyecandy::math::Matrix<size_type, 1, 2>;

    LineTracer(const Point& a, const Point& b)
        : width_{std::abs(b.x() - a.x())},
          height_{std::abs(b.y() - a.y())},
          dx_{a.x() < b.x() ? 1 : -1},
          dy_{a.y() < b.y() ? 1 : -1},
          error_{height_ <= width_ ? 2 * height_ - width_ : height_ - 2 * width_},
          end_{height_ <= width_ ? b.x() : b.y()},
          x_{a.x()},
          y_{a.y()} {}

    auto step() {
        if (height_ <= width_ && x_ != end_) {
            if (error_ > 0) {
                error_ -= 2 * width_;
                y_ += dy_;
            }
            error_ += 2 * height_;
            x_ += dx_;
            return true;
        } else if (height_ > width_ && y_ != end_) {
            if (error_ < 0) {
                error_ += 2 * height_;
                x_ += dx_;
            }
            error_ -= 2 * width_;
            y_ += dy_;
            return true;
        }
        return false;
    }

    auto position() const { return Point{{x_, y_}}; }

private:
    size_type width_;
    size_type height_;
    size_type dx_;
    size_type dy_;
    size_type error_;
    size_type end_;
    size_type x_;
    size_type y_;
};
}
}
}
