#pragma once

#include "dansandu/eyecandy/math/matrix.hpp"
#include "dansandu/eyecandy/math/numeric_traits.hpp"

#include <cmath>

namespace dansandu {
namespace eyecandy {
namespace math {

template<typename T>
Matrix<T> identity(typename Matrix<T>::size_type n) {
    Matrix<T> result(n, n);
    for (auto i = 0; i < n; ++i)
        result(i, i) = multiplicative_identity<T>;
    return result;
}

template<int index, typename T>
void scalingWork(Matrix<T>& matrix) {
    matrix(index, index) = multiplicative_identity<T>;
}

template<int index, typename T, typename A, typename... AA>
void scalingWork(Matrix<T>& matrix, A arg, AA... args) {
    matrix(index, index) = arg;
    scalingWork<index + 1, T>(matrix, std::forward<AA>(args)...);
}

template<typename T, typename... AA>
Matrix<T> scaling(AA... args) {
    constexpr auto size = sizeof...(AA) + 1;
    Matrix<T> result(size, size);
    scalingWork<0, T>(result, std::forward<AA>(args)...);
    return result;
}

template<int index, typename T>
void translationWork(Matrix<T>&) {}

template<int index, typename T, typename A, typename... AA>
void translationWork(Matrix<T>& matrix, A arg, AA... args) {
    matrix(index, matrix.columns() - 1) = arg;
    translationWork<index + 1, T>(matrix, std::forward<AA>(args)...);
}

template<typename T, typename... AA>
Matrix<T> translation(AA... args) {
    auto result = identity<T>(sizeof...(AA) + 1);
    translationWork<0, T>(result, std::forward<AA>(args)...);
    return result;
}

template<typename T>
Matrix<T> rotationByX(T radians) {
    auto result = Matrix<T>(4, 4);
    result(0, 0) = result(3, 3) = multiplicative_identity<T>;
    auto cos = std::cos(radians);
    auto sin = std::sin(radians);
    result(1, 1) = cos;
    result(1, 2) = -sin;
    result(2, 1) = sin;
    result(2, 2) = cos;
    return result;
}
}
}
}
