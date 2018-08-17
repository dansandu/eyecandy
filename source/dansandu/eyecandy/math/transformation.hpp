#pragma once

#include "Eigen/Core"
#include "dansandu/eyecandy/math/numeric_traits.hpp"

#include <cmath>

namespace dansandu {
namespace eyecandy {
namespace math {

template<typename T>
auto scaling(T x, T y, T z) {
    constexpr auto _1 = multiplicative_identity<T>;
    constexpr auto _0 = additive_identity<T>;
    Eigen::Matrix<T, 4, 4> result;
    // clang-format off
    result <<  x, _0, _0, _0,
              _0,  y, _0, _0,
              _0, _0,  z, _0,
              _0, _0, _0, _1;
    // clang-format on
    return result;
}

template<typename T>
auto translation(T x, T y, T z) {
    constexpr auto _1 = multiplicative_identity<T>;
    constexpr auto _0 = additive_identity<T>;
    Eigen::Matrix<T, 4, 4> matrix;
    // clang-format off
    matrix << _1, _0, _0,  x,
              _0, _1, _0,  y,
              _0, _0, _1,  z,
              _0, _0, _0, _1;
    // clang-format on
    return matrix;
}

template<typename T>
auto rotationByX(T radians) {
    constexpr auto _1 = multiplicative_identity<T>;
    constexpr auto _0 = additive_identity<T>;
    auto cos = std::cos(radians);
    auto sin = std::sin(radians);
    Eigen::Matrix<T, 4, 4> matrix;
    // clang-format off
    matrix << _1,  _0,   _0, _0,
              _0, cos, -sin, _0,
              _0, sin,  cos, _0,
              _0,  _0,   _0, _1;
    // clang-format on
    return matrix;
}

template<typename T>
auto rotationByY(T radians) {
    constexpr auto _1 = multiplicative_identity<T>;
    constexpr auto _0 = additive_identity<T>;
    auto cos = std::cos(radians);
    auto sin = std::sin(radians);
    Eigen::Matrix<T, 4, 4> matrix;
    // clang-format off
    matrix <<  cos, _0, sin, _0,
                _0, _1,  _0, _0,
              -sin, _0, cos, _0,
                _0, _0,  _0, _1;
    // clang-format on
    return matrix;
}

template<typename T>
auto rotationByZ(T radians) {
    constexpr auto _1 = multiplicative_identity<T>;
    constexpr auto _0 = additive_identity<T>;
    auto cos = std::cos(radians);
    auto sin = std::sin(radians);
    Eigen::Matrix<T, 4, 4> matrix;
    // clang-format off
    matrix << cos, -sin, _0, _0,
              sin,  cos, _0, _0,
               _0,   _0, _1, _0,
               _0,   _0, _0, _1;
    // clang-format on
    return matrix;
}

template<typename T>
Eigen::Matrix<T, 4, 4> lookAt(const Eigen::Matrix<T, 4, 1>& eye, const Eigen::Matrix<T, 4, 1>& target,
                              const Eigen::Matrix<T, 4, 1>& up) {
    constexpr auto _1 = multiplicative_identity<T>;
    constexpr auto _0 = additive_identity<T>;
    auto z = (eye - target).normalized();
    auto x = up.cross(z).normalized();
    auto y = z.cross(x);
    Eigen::Matrix<T, 4, 4> result;
    result << x, y, z, _0, _0, _0, _1;
    return result * translation(-eye(0), -eye(1), -eye(2));
}

template<typename T, int M, int N>
bool closeTo(const Eigen::Matrix<T, M, N>& lhs, const Eigen::Matrix<T, M, N>& rhs, T epsilon) {
    return (lhs - rhs).norm() <= epsilon;
}
}
}
}
