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
    return Matrix4<T>{{{ x, _0, _0, _0},
                       {_0,  y, _0, _0},
                       {_0, _0,  z, _0},
                       {_0, _0, _0, _1}}};
}

template<typename T>
auto translation(T x, T y, T z) {
    constexpr auto _1 = multiplicative_identity<T>;
    constexpr auto _0 = additive_identity<T>;
    return Matrix4<T>{{{_1, _0, _0, _0},
                       {_0, _1, _0, _0},
                       {_0, _0, _1, _0},
                       { x,  y,  z, _1}}};
}

template<typename T>
auto translation(const RowVector3<T>& vector) {
    return translation(vector.x(), vector.y(), vector.z());
}

template<typename T>
auto rotationByX(T radians) {
    constexpr auto _1 = multiplicative_identity<T>;
    constexpr auto _0 = additive_identity<T>;
    auto cos = std::cos(radians);
    auto sin = std::sin(radians);
    return Matrix4<T>{{{_1,   _0,  _0, _0},
                       {_0,  cos, sin, _0},
                       {_0, -sin, cos, _0},
                       {_0,   _0,  _0, _1}}};
}

template<typename T>
auto rotationByY(T radians) {
    constexpr auto _1 = multiplicative_identity<T>;
    constexpr auto _0 = additive_identity<T>;
    auto cos = std::cos(radians);
    auto sin = std::sin(radians);
    return Matrix4<T>{{{cos, _0, -sin, _0},
                       { _0, _1,   _0, _0},
                       {sin, _0,  cos, _0},
                       { _0, _0,   _0, _1}}};
}

template<typename T>
auto rotationByZ(T radians) {
    constexpr auto _1 = multiplicative_identity<T>;
    constexpr auto _0 = additive_identity<T>;
    auto cos = std::cos(radians);
    auto sin = std::sin(radians);
    return Matrix4<T>{{{ cos, sin, _0, _0},
                       {-sin, cos, _0, _0},
                       {  _0,  _0, _1, _0},
                       {  _0,  _0, _0, _1}}};
}

template<typename T>
auto lookAt(const RowVector3<T>& eye, const RowVector3<T>& target, const RowVector3<T>& up) {
    constexpr auto _1 = multiplicative_identity<T>;
    constexpr auto _0 = additive_identity<T>;
    auto w = normalized(eye - target);
    auto u = normalized(crossProduct(up, w));
    auto v = crossProduct(w, u);
    return translation(-eye) * Matrix4<T>{{{u.x(), v.x(), w.x(), _0},
                                           {u.y(), v.y(), w.y(), _0},
                                           {u.z(), v.z(), w.z(), _0},
                                           {   _0,    _0,    _0, _1}}};
}

template<typename T>
auto perspective(T near, T far, T fieldOfViewRadians, T aspect) {
    constexpr auto _1 = multiplicative_identity<T>;
    constexpr auto _0 = additive_identity<T>;
    auto ctg = _1 / std::tan(0.5 * fieldOfViewRadians);
    auto p33 = (far + near) / (near - far);
    auto p43 = 2.0 * far * near / (near - far);

    return Matrix4<T>{{{ctg,           _0,  _0,  _0}, 
                       { _0, aspect * ctg,  _0,  _0},
                       { _0,           _0, p33, -_1},
                       { _0,           _0, p43,  _0}}};
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
