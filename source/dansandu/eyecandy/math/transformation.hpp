#pragma once

#include "dansandu/eyecandy/math/matrix.hpp"
#include "dansandu/eyecandy/math/numeric_traits.hpp"

#include <cmath>

namespace dansandu {
namespace eyecandy {
namespace math {

// clang-format off

template<typename T>
auto scaling(T x, T y, T z) {
    constexpr auto _1 = multiplicative_identity<T>;
    constexpr auto _0 = additive_identity<T>;

    return Matrix<T>{{ x, _0, _0, _0},
                     {_0,  y, _0, _0},
                     {_0, _0,  z, _0},
                     {_0, _0, _0, _1}};
}

template<typename T>
auto translation(T x, T y, T z) {
    constexpr auto _1 = multiplicative_identity<T>;
    constexpr auto _0 = additive_identity<T>;

    return Matrix<T>{{_1, _0, _0, _0},
                     {_0, _1, _0, _0},
                     {_0, _0, _1, _0},
                     { x,  y,  z, _1}};
}

template<typename T>
auto translation(const Matrix<T>& matrix) {
    if (matrix.rows() * matrix.columns() != 3)
        throw std::invalid_argument{"matrix must be row or column vector"};

    return translation(matrix(0), matrix(1), matrix(2));
}

template<typename T>
auto rotationByX(T radians) {
    constexpr auto _1 = multiplicative_identity<T>;
    constexpr auto _0 = additive_identity<T>;
    auto cos = std::cos(radians);
    auto sin = std::sin(radians);

    return Matrix<T>{{_1,   _0,  _0, _0},
                     {_0,  cos, sin, _0},
                     {_0, -sin, cos, _0},
                     {_0,   _0,  _0, _1}};
}

template<typename T>
auto rotationByY(T radians) {
    constexpr auto _1 = multiplicative_identity<T>;
    constexpr auto _0 = additive_identity<T>;
    auto cos = std::cos(radians);
    auto sin = std::sin(radians);

    return Matrix<T>{{cos, _0, -sin, _0},
                     { _0, _1,   _0, _0},
                     {sin, _0,  cos, _0},
                     { _0, _0,   _0, _1}};
}

template<typename T>
auto rotationByZ(T radians) {
    constexpr auto _1 = multiplicative_identity<T>;
    constexpr auto _0 = additive_identity<T>;
    auto cos = std::cos(radians);
    auto sin = std::sin(radians);

    return Matrix<T>{{ cos, sin, _0, _0},
                     {-sin, cos, _0, _0},
                     {  _0,  _0, _1, _0},
                     {  _0,  _0, _0, _1}};
}

template<typename T>
auto lookAt(const Matrix<T>& eye, const Matrix<T>& target, const Matrix<T>& up) {
    constexpr auto _1 = multiplicative_identity<T>;
    constexpr auto _0 = additive_identity<T>;
    auto z = normalized(eye - target);
    auto x = normalized(crossProduct(up, z));
    auto y = crossProduct(z, x);

    return translation(-eye) *
           Matrix<T>{{x(0), y(0), z(0), _0},
                     {x(1), y(1), z(1), _0},
                     {x(2), y(2), z(2), _0},
                     {  _0,   _0,   _0, _1}};
}

template<typename T>
auto perspective(T near, T far, T fieldOfViewRadians, T aspect) {
    constexpr auto _1 = multiplicative_identity<T>;
    constexpr auto _0 = additive_identity<T>;
    auto ctg = _1 / std::tan(0.5 * fieldOfViewRadians);
    auto p33 = (far + near) / (near - far);
    auto p43 = 2.0 * far * near / (near - far);

    return Matrix<T>{{ctg,           _0,  _0,  _0}, 
                     { _0, aspect * ctg,  _0,  _0},
                     { _0,           _0, p33, -_1},
                     { _0,           _0, p43,  _0}};
}

template<typename T>
auto viewport(T width, T height) {
    constexpr auto _1 = multiplicative_identity<T>;
    return scaling<T>(width / 2, height / 2, _1) * translation<T>(width / 2, height / 2, -_1);
}

// clang-format on
}
}
}
