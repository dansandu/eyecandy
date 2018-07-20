#pragma once

#include <cmath>

namespace dansandu {
namespace eyecandy {
namespace math {

template<typename T>
constexpr T get_pi() {
    return 3.141592653589793238462643383279502884197169399375105820974944592307816406286L;
}

template<typename T>
struct numeric_traits {};

template<>
struct numeric_traits<int> {
    static constexpr int addition_identity = 0;
    static constexpr int multiplicative_identity = 1;
    static constexpr int pi = get_pi<int>();
};

template<>
struct numeric_traits<float> {
    static constexpr float addition_identity = 0.0F;
    static constexpr float multiplicative_identity = 1.0F;
    static constexpr float pi = get_pi<float>();
};

template<>
struct numeric_traits<double> {
    static constexpr double addition_identity = 0.0;
    static constexpr double multiplicative_identity = 1.0;
    static constexpr double pi = get_pi<double>();
};

template<typename T>
constexpr auto addition_identity = numeric_traits<T>::addition_identity;

template<typename T>
constexpr auto multiplicative_identity = numeric_traits<T>::multiplicative_identity;

template<typename T>
constexpr auto pi = numeric_traits<T>::pi;

template<typename T>
constexpr auto closeTo(T lhs, T rhs, T epsilon) {
    return std::abs(lhs - rhs) <= epsilon;
}
}
}
}
