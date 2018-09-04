#include "dansandu/eyecandy/math/point.hpp"

namespace dansandu {
namespace eyecandy {
namespace raster {

class LineTracer {
public:
    using Point = dansandu::eyecandy::math::Point;

    LineTracer(Point a, Point b)
        : width_{std::abs(b.x - a.x)},
          height_{std::abs(b.y - a.y)},
          dx_{a.x < b.x ? 1 : -1},
          dy_{a.y < b.y ? 1 : -1},
          error_{height_ <= width_ ? 2 * height_ - width_ : height_ - 2 * width_},
          end_{height_ <= width_ ? b.x : b.y},
          x_{a.x},
          y_{a.y} {}

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

    Point position() const { return {x_, y_}; }

private:
    int width_;
    int height_;
    int dx_;
    int dy_;
    int error_;
    int end_;
    int x_;
    int y_;
};
}
}
}
