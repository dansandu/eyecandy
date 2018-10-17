#pragma once

#include "dansandu/eyecandy/math/matrix.hpp"
#include "dansandu/eyecandy/math/numeric_traits.hpp"

namespace dansandu {
namespace eyecandy {
namespace math {

template<typename T>
auto interpolate(const RowVector3<T>& a, const RowVector3<T>& b, T x, T y, T epsilon) {
    auto t = a.z() < b.z() ? additive_identity<T> : multiplicative_identity<T>;
    if (!closeTo(a.x(), b.x(), epsilon))
        t = (x - a.x()) / (b.x() - a.x());
    else if (!closeTo(a.y(), b.y(), epsilon))
        t = (y - a.y()) / (b.y() - a.y());

    return a + t * (b - a);
}

template<typename T>
auto interpolate(const RowVector3<T>& a, const RowVector3<T>& b, const RowVector3<T>& c, T x, T y, T epsilon) {
    auto n = crossProduct(a - c, b - c);
    auto z = closeTo(n.z(), additive_identity<T>, epsilon) ? std::min(a.z(), std::min(b.z(), c.z()))
                                                           : (n.x() * x + n.y() * y - dotProduct(c, n)) / -n.z();
    return RowVector3<T>{{x, y, z}};
}
}
}
}
