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
    static constexpr int additive_identity = 0;
    static constexpr int multiplicative_identity = 1;
    static constexpr int pi = get_pi<int>();
};

template<>
struct numeric_traits<float> {
    static constexpr float additive_identity = 0.0F;
    static constexpr float multiplicative_identity = 1.0F;
    static constexpr float pi = get_pi<float>();
};

template<>
struct numeric_traits<double> {
    static constexpr double additive_identity = 0.0;
    static constexpr double multiplicative_identity = 1.0;
    static constexpr double pi = get_pi<double>();
};

template<typename T>
constexpr auto additive_identity = numeric_traits<T>::additive_identity;

template<typename T>
constexpr auto multiplicative_identity = numeric_traits<T>::multiplicative_identity;

template<typename T>
constexpr auto pi = numeric_traits<T>::pi;

template<typename T>
constexpr auto closeTo(T lhs, T rhs, T epsilon) {
    return std::abs(lhs - rhs) <= epsilon;
}

template<typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
constexpr auto interval(T value, T intervalBegin, T intervalExclusiveEnd) {
    return intervalBegin <= value && value < intervalExclusiveEnd;
}

template<typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
constexpr auto subinterval(T subintervalBegin, T subintervalExclusiveEnd, T intervalBegin, T intervalExclusiveEnd) {
    return intervalBegin <= subintervalBegin && subintervalBegin < intervalExclusiveEnd &&
           intervalBegin < subintervalExclusiveEnd && subintervalExclusiveEnd <= intervalExclusiveEnd;
}
}
}
}
