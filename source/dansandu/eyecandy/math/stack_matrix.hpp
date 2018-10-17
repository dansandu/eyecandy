#pragma once

#include "dansandu/eyecandy/math/base_matrix.hpp"
#include "dansandu/eyecandy/math/numeric_traits.hpp"
#include "dansandu/eyecandy/utility/exception.hpp"

#include <algorithm>
#include <cmath>
#include <functional>
#include <type_traits>

namespace dansandu {
namespace eyecandy {
namespace math {

template<typename T, size_type staticRows, size_type staticColumns>
class Matrix<T, staticRows, staticColumns, StorageStrategy::stack> {
public:
    using value_type = T;

private:
    static_assert(staticRows > 0, "stack matrix rows must be positive and non-dynamic");
    static_assert(staticColumns > 0, "stack matrix columns must be positive and non-dynamic");

    constexpr static size_type rows_{staticRows};
    constexpr static size_type columns_{staticColumns};
    constexpr static size_type staticSize_{staticRows * staticColumns};
    constexpr static auto isStaticVector_{staticRows == 1 || staticColumns == 1};

public:
    explicit constexpr Matrix(value_type fillValue = dansandu::eyecandy::math::additive_identity<value_type>) {
        std::fill(begin(), end(), fillValue);
    }

    template<typename U = size_type, size_type length,
             typename = std::enable_if_t<isStaticVector_ && staticSize_ == length, U>>
    explicit constexpr Matrix(const value_type (&array)[length]) {
        std::copy(std::begin(array), std::end(array), begin());
    }

    template<typename U = size_type, size_type rowCount, size_type columnCount,
             typename = std::enable_if_t<rows_ == rowCount && columns_ == columnCount, U>>
    explicit constexpr Matrix(const value_type (&array)[rowCount][columnCount]) {
        auto position = begin();
        for (auto row = 0; row < rowCount; ++row)
            for (auto column = 0; column < columnCount; ++column)
                *position++ = array[row][column];
    }

    constexpr Matrix(size_type, size_type) : Matrix{} {}

    constexpr Matrix(const Matrix&) = default;

    constexpr Matrix(Matrix&&) noexcept = default;

    constexpr Matrix& operator=(const Matrix&) = default;

    constexpr Matrix& operator=(Matrix&&) noexcept = default;

    constexpr Matrix& operator+=(const Matrix& other) {
        std::transform(begin(), end(), other.begin(), begin(), std::plus<T>{});
        return *this;
    }

    constexpr Matrix& operator-=(const Matrix& other) {
        std::transform(begin(), end(), other.begin(), begin(), std::minus<T>{});
        return *this;
    }

    constexpr Matrix& operator*=(T scalar) {
        for (auto& element : data_)
            element *= scalar;
        return *this;
    }

    constexpr auto swap(Matrix& other) noexcept {
        using std::swap;
        for (auto i = 0; i < columns_; ++i)
            swap(data_[i], other.data_[i]);
    }

    template<typename U = size_type, typename = std::enable_if_t<isStaticVector_ && staticSize_ >= 1, U>>
    constexpr auto& x() {
        return data_[0];
    }

    template<typename U = size_type, typename = std::enable_if_t<isStaticVector_ && staticSize_ >= 2, U>>
    constexpr auto& y() {
        return data_[1];
    }

    template<typename U = size_type, typename = std::enable_if_t<isStaticVector_ && staticSize_ >= 3, U>>
    constexpr auto& z() {
        return data_[2];
    }

    template<typename U = size_type, typename = std::enable_if_t<isStaticVector_ && staticSize_ >= 4, U>>
    constexpr auto& w() {
        return data_[3];
    }

    template<typename U = value_type, typename = std::enable_if_t<isStaticVector_, U>>
    auto& operator()(size_type n) {
        return data_[index(n)];
    }

    auto& operator()(size_type row, size_type column) { return data_[index(row, column)]; }

    constexpr auto begin() { return std::begin(data_); }

    constexpr auto end() { return std::end(data_); }

    template<typename U = value_type, typename = std::enable_if_t<isStaticVector_, U>>
    auto operator()(size_type n) const {
        return data_[index(n)];
    }

    auto operator()(size_type row, size_type column) const { return data_[index(row, column)]; }

    template<typename U = size_type, typename = std::enable_if_t<isStaticVector_ && staticSize_ >= 1, U>>
    constexpr auto x() const {
        return data_[0];
    }

    template<typename U = size_type, typename = std::enable_if_t<isStaticVector_ && staticSize_ >= 2, U>>
    constexpr auto y() const {
        return data_[1];
    }

    template<typename U = size_type, typename = std::enable_if_t<isStaticVector_ && staticSize_ >= 3, U>>
    constexpr auto z() const {
        return data_[2];
    }

    template<typename U = size_type, typename = std::enable_if_t<isStaticVector_ && staticSize_ >= 4, U>>
    constexpr auto w() const {
        return data_[3];
    }

    constexpr auto begin() const { return std::begin(data_); }

    constexpr auto end() const { return std::end(data_); }

    constexpr auto rows() const { return rows_; }

    constexpr auto columns() const { return columns_; }

    constexpr auto closeTo(const Matrix& other, T epsilon) {
        return std::equal(begin(), end(), other.begin(), other.end(),
                          [epsilon](auto a, auto b) { return dansandu::eyecandy::math::closeTo(a, b, epsilon); });
    }

private:
    template<typename U = value_type, typename = std::enable_if_t<isStaticVector_, U>>
    auto index(size_type n) const {
        if (n < 0 || n >= rows_ * columns_)
            THROW(std::out_of_range, "cannot index the #th element in a #-length vector", n, rows_ * columns_);

        return n;
    }

    auto index(size_type row, size_type column) const {
        if (row < 0 || row >= rows_ || column < 0 || column >= columns_)
            THROW(std::out_of_range,
                  "cannot index the #th row and #th column in a #x# matrix -- indices are out of bounds", row, column,
                  rows_, columns_);

        return row * columns_ + column;
    }

    value_type data_[rows_ * columns_];
};
}
}
}
