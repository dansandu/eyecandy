#pragma once

#include <algorithm>
#include <ostream>

namespace dansandu {
namespace eyecandy {
namespace math {

struct Point {
    using value_type = int;

    Point() : x{0}, y{0} {}

    Point(value_type x, value_type y) : x{x}, y{y} {}

    auto swap(Point& other) noexcept {
        using std::swap;
        swap(x, other.x);
        swap(y, other.y);
    }

    auto yXorderSmallerThan(Point other) { return y < other.y || (y == other.y && x < other.x); }

    int x;
    int y;
};

inline auto swap(Point& a, Point& b) noexcept { a.swap(b); }

inline auto sortPointsByYx(Point& a, Point& b, Point& c) {
    using std::swap;
    if (a.yXorderSmallerThan(b)) {
        if (c.yXorderSmallerThan(a))
            swap(a, c);
    } else {
        if (b.yXorderSmallerThan(c))
            swap(a, b);
        else
            swap(a, c);
    }
    if (c.yXorderSmallerThan(b))
        swap(b, c);
}

inline auto operator==(Point a, Point b) { return a.x == b.x && a.y == b.y; }

inline auto operator!=(Point a, Point b) { return !(a == b); }

inline auto& operator<<(std::ostream& os, Point point) { return os << "(" << point.x << ", " << point.y << ")"; }
}
}
}
