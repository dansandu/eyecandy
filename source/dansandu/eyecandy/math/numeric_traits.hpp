#pragma once

#include <cmath>

namespace dansandu {
namespace eyecandy {
namespace math {

template<typename T>
struct numeric_traits {};

template<>
struct numeric_traits<float> {
    static constexpr float addition_identity = 0.0F;
    static constexpr float multiplicative_identity = 1.0F;

    static constexpr bool equal(float lhs, float rhs, float epsilon) { return std::abs(lhs - rhs) < epsilon; }
};

template<>
struct numeric_traits<double> {
    static constexpr double addition_identity = 0.0;
    static constexpr double multiplicative_identity = 1.0;

    static constexpr bool equal(double lhs, double rhs, double epsilon) { return std::abs(lhs - rhs) < epsilon; }
};

template<typename T>
constexpr auto addition_identity = numeric_traits<T>::addition_identity;

template<typename T>
constexpr auto multiplicative_identity = numeric_traits<T>::multiplicative_identity;

template<typename T>
constexpr auto equal(T lhs, T rhs, T epsilon) {
    return numeric_traits<T>::equal(lhs, rhs, epsilon);
}
}
}
}
