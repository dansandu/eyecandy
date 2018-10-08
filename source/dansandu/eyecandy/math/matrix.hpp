#pragma once

#include "dansandu/eyecandy/math/base_matrix.hpp"
#include "dansandu/eyecandy/math/heap_matrix.hpp"
#include "dansandu/eyecandy/math/matrix_slice.hpp"
#include "dansandu/eyecandy/math/numeric_traits.hpp"
#include "dansandu/eyecandy/math/stack_matrix.hpp"
#include "dansandu/eyecandy/utility/exception.hpp"

#include <algorithm>
#include <cmath>
#include <functional>
#include <iomanip>
#include <sstream>
#include <type_traits>

namespace dansandu {
namespace eyecandy {
namespace math {

template<typename T, size_type M, size_type N>
auto closeTo(Matrix<T, M, N>& a, Matrix<T, M, N>& b, T epsilon) {
    return a.closeTo(b, epsilon);
}

template<typename T, size_type M, size_type N>
auto swap(Matrix<T, M, N>& a, Matrix<T, M, N>& b) {
    a.swap(b);
}

template<typename T, size_type M, size_type N>
auto operator+(const Matrix<T, M, N>& a, const Matrix<T, M, N> b) {
    auto result = a;
    return result += b;
}

template<typename T, size_type M, size_type N>
auto operator-(const Matrix<T, M, N>& a, const Matrix<T, M, N> b) {
    auto result = a;
    return result -= b;
}

template<typename T, size_type M, size_type N>
auto operator*(const Matrix<T, M, N>& matrix, T scalar) {
    auto result = matrix;
    return result *= scalar;
}

template<typename T, size_type M, size_type N>
auto operator*(T scalar, const Matrix<T, M, N>& matrix) {
    auto result = matrix;
    return result *= scalar;
}

template<typename T, size_type M, size_type N, typename = std::enable_if_t<std::is_integral_v<T>>>
auto operator==(const Matrix<T, M, N>& a, const Matrix<T, M, N> b) {
    return a.rows() == b.rows() && a.columns() && b.columns() && std::equal(std::begin(a), std::end(a), std::begin(b));
}

template<typename T, size_type M, size_type N, typename = std::enable_if_t<std::is_integral_v<T>>>
auto operator!=(const Matrix<T, M, N>& a, const Matrix<T, M, N> b) {
    return !(a == b);
}

template<typename T, size_type M, size_type N, size_type K>
auto operator*(const Matrix<T, M, N>& a, const Matrix<T, N, K>& b) {
    Matrix<T, M, K> result{a.rows(), b.columns()};
    for (auto i = 0; i < a.rows(); ++i)
        for (auto k = 0; k < b.columns(); ++k)
            for (auto j = 0; j < a.columns(); ++j)
                result(i, k) += a(i, j) * b(j, k);

    if constexpr (M == 1 && K == 1)
        return result.x();
    else
        return result;
}

template<typename T, size_type M, size_type N>
auto operator-(const Matrix<T, M, N>& matrix) {
    constexpr auto _1 = dansandu::eyecandy::math::multiplicative_identity<T>;
    return -_1 * matrix;
}

template<typename T, size_type M, size_type N, typename = std::enable_if_t<M == 1 || N == 1, T>>
auto toString(const Matrix<T, M, N>& vector, size_type decimalPoints = 2) {
    std::stringstream ss;
    ss << std::fixed << std::setprecision(decimalPoints) << "(";
    auto i = 0;
    for (; i + 1 < std::max(M, N); ++i)
        ss << vector(i) << ", ";
    ss << vector(i) << ")";
    return ss.str();
}

template<typename T, size_type M, size_type N, typename = std::enable_if_t<M == 1 || N == 1, T>>
auto normalized(const Matrix<T, M, N>& vector) {
    auto _1 = dansandu::eyecandy::math::multiplicative_identity<T>;
    return (_1 / magnitude(vector)) * vector;
}

template<typename T, size_type M, size_type N, size_type K, size_type L,
         typename = std::enable_if_t<(M == 1 || N == 1) && (K == 1 || L == 1) && M * N == K * L, T>>
auto dotProduct(const Matrix<T, M, N>& a, const Matrix<T, K, L>& b) {
    auto sum = dansandu::eyecandy::math::additive_identity<T>;
    for (auto left = a.begin(), right = b.begin(); left != a.end(); ++left, ++right)
        sum += *left * *right;
    return sum;
}

template<typename T, size_type M, size_type N, typename = std::enable_if_t<M == 1 || N == 1, T>>
auto magnitude(const Matrix<T, M, N>& vector) {
    return static_cast<T>(std::sqrt(dotProduct(vector, vector)));
}

template<typename T, size_type M, size_type N, typename = std::enable_if_t<(M == 1 || N == 1) && M * N == 3, T>>
auto crossProduct(const Matrix<T, M, N>& a, const Matrix<T, M, N>& b) {
    auto x = a.y() * b.z() - b.y() * a.z();
    auto y = b.x() * a.z() - a.x() * b.z();
    auto z = a.x() * b.y() - b.x() * a.y();
    return Matrix<T, M, N>{{x, y, z}};
}

template<typename U, typename T, size_type M, size_type N>
auto matrix_cast(const Matrix<T, M, N>& matrix) {
    Matrix<U, M, N> result;
    std::transform(std::begin(matrix), std::end(matrix), std::begin(result),
                   [](auto element) { return static_cast<U>(element); });
    return result;
}

template<typename T, size_type M>
auto dehomogenize(Matrix<T, M, 4>& matrix) {
    for (auto i = 0; i < matrix.rows(); ++i)
        for (auto j = 0; j < matrix.columns(); ++j)
            matrix(i, j) /= matrix(i, matrix.columns() - 1);
}

template<typename T>
using RowVector2 = Matrix<T, 1, 2>;

template<typename T>
using RowVector3 = Matrix<T, 1, 3>;

template<typename T>
using RowVector4 = Matrix<T, 1, 4>;

template<typename T>
using ColumnVector3 = Matrix<T, 3, 1>;

template<typename T>
using ColumnVector4 = Matrix<T, 4, 1>;

template<typename T>
using Matrix4 = Matrix<T, 4, 4>;
}
}
}
