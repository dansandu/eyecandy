#pragma once

#include "dansandu/eyecandy/math/base_matrix.hpp"
#include "dansandu/eyecandy/math/numeric_traits.hpp"
#include "dansandu/eyecandy/utility/exception.hpp"

#include <type_traits>
#include <vector>

namespace dansandu {
namespace eyecandy {
namespace math {

template<typename T, size_type staticRows, size_type staticColumns>
class Matrix<T, staticRows, staticColumns, StorageStrategy::heap> {
public:
    using value_type = T;

private:
    static_assert(staticRows > 0 || staticRows == dynamic, "matrix rows must be positive or dynamic");
    static_assert(staticColumns > 0 || staticColumns == dynamic, "matrix columns must be positive or dynamic");

    constexpr static size_type staticSize_{staticRows * staticColumns};
    constexpr static auto isStaticVector_{staticRows == 1 || staticColumns == 1};

public:
    template<typename U = value_type, typename = std::enable_if_t<staticRows == dynamic || staticColumns == dynamic, U>>
    Matrix() : rows_{staticRows == dynamic ? 0 : staticRows}, columns_{staticColumns == dynamic ? 0 : staticColumns} {}

    template<typename U = value_type, typename = std::enable_if_t<staticRows != dynamic && staticColumns != dynamic, U>>
    explicit Matrix(value_type fillValue = dansandu::eyecandy::math::additive_identity<value_type>)
        : rows_{staticRows}, columns_{staticColumns}, data_(rows_ * columns_, fillValue) {}

    template<typename U = value_type, size_type length,
             typename = std::enable_if_t<isStaticVector_ && staticSize_ == length && length >= 1, U>>
    explicit Matrix(const value_type (&array)[length])
        : rows_{staticRows}, columns_{staticColumns}, data_(std::begin(array), std::end(array)) {}

    template<typename U = value_type, size_type rowCount, size_type columnCount,
             typename = std::enable_if_t<
                 staticRows == rowCount && staticColumns == columnCount && rowCount >= 1 && columnCount >= 1, U>>
    explicit Matrix(const value_type (&array)[rowCount][columnCount]) : rows_{rowCount}, columns_{columnCount} {
        data_.reserve(rowCount * columnCount);
        for (auto row = 0; row < rowCount; ++row)
            for (auto column = 0; column < columnCount; ++column)
                data_.push_back(array[row][column]);
    }

    template<typename U = value_type, typename = std::enable_if_t<staticRows == dynamic || staticColumns == dynamic, U>>
    Matrix(size_type rowCount, size_type columnCount, std::vector<value_type> buffer)
        : rows_{rowCount}, columns_{columnCount}, data_{std::move(buffer)} {
        if (rows_ < 0 || columns_ < 0)
            THROW(std::invalid_argument, "invalid dimensions ", rows_, "x", columns_,
                  " provided in matrix constructor -- rows and columns must be greater than or equal to zero");

        if ((staticRows != rows_ && staticRows != dynamic) || (staticColumns != columns_ && staticColumns != dynamic))
            THROW(std::invalid_argument, "static dimensions ", staticRows, "x", staticColumns,
                  " do not match dynamic dimensions ", rows_, "x", columns_);

        if (rows_ * columns_ != static_cast<size_type>(data_.size()))
            THROW(std::invalid_argument, "buffer size ", data_.size(), " doesn't match matrix size ", rows_ * columns_);
    }

    Matrix(size_type rowCount, size_type columnCount,
           value_type fillValue = dansandu::eyecandy::math::additive_identity<value_type>)
        : rows_{rowCount}, columns_{columnCount} {
        if (rows_ < 0 || columns_ < 0)
            THROW(std::invalid_argument, "invalid dimensions ", rows_, "x", columns_,
                  " provided in matrix constructor -- rows and columns must be greater than or equal to zero");

        if ((staticRows != rows_ && staticRows != dynamic) || (staticColumns != columns_ && staticColumns != dynamic))
            THROW(std::invalid_argument, "static dimensions ", staticRows, "x", staticColumns,
                  " do not match dynamic dimensions ", rows_, "x", columns_);

        data_ = std::vector<value_type>(rows_ * columns_, fillValue);
    }

    Matrix(const Matrix&) = default;

    Matrix(Matrix&&) noexcept = default;

    Matrix& operator=(const Matrix&) = default;

    Matrix& operator=(Matrix&&) noexcept = default;

    Matrix& operator+=(const Matrix& other) {
        if (rows_ != other.rows_ || columns_ != other.columns_)
            THROW(std::invalid_argument, "cannot add right hand side ", other.rows_, "x", other.columns_,
                  " matrix to left hand side ", rows_, "x", columns_, " matrix -- matrix dimensions must match");

        std::transform(begin(), end(), other.begin(), begin(), std::plus<T>{});
        return *this;
    }

    Matrix& operator-=(const Matrix& other) {
        if (rows_ != other.rows_ || columns_ != other.columns_)
            THROW(std::invalid_argument, "cannot subtract right hand side ", other.rows_, "x", other.columns_,
                  " matrix from left hand side ", rows_, "x", columns_, " matrix -- matrix dimensions must match");

        std::transform(begin(), end(), other.begin(), begin(), std::minus<T>{});
        return *this;
    }

    Matrix& operator*=(T scalar) {
        for (auto& element : data_)
            element *= scalar;
        return *this;
    }

    auto swap(Matrix& other) noexcept {
        using std::swap;
        swap(rows_, other.rows_);
        swap(columns_, other.columns_);
        swap(data_, other.data_);
    }

    template<typename U = size_type, typename = std::enable_if_t<isStaticVector_ && staticSize_ >= 1, U>>
    auto& x() {
        return data_[0];
    }

    template<typename U = size_type, typename = std::enable_if_t<isStaticVector_ && staticSize_ >= 2, U>>
    auto& y() {
        return data_[1];
    }

    template<typename U = size_type, typename = std::enable_if_t<isStaticVector_ && staticSize_ >= 3, U>>
    auto& z() {
        return data_[2];
    }

    template<typename U = size_type, typename = std::enable_if_t<isStaticVector_ && staticSize_ >= 4, U>>
    auto& w() {
        return data_[3];
    }

    auto& operator()(size_type row, size_type column) { return data_[index(row, column)]; }

    auto begin() { return std::begin(data_); }

    auto end() { return std::end(data_); }

    template<typename U = size_type, typename = std::enable_if_t<isStaticVector_ && staticSize_ >= 1, U>>
    auto& x() const {
        return data_[0];
    }

    template<typename U = size_type, typename = std::enable_if_t<isStaticVector_ && staticSize_ >= 2, U>>
    auto& y() const {
        return data_[1];
    }

    template<typename U = size_type, typename = std::enable_if_t<isStaticVector_ && staticSize_ >= 3, U>>
    auto& z() const {
        return data_[2];
    }

    template<typename U = size_type, typename = std::enable_if_t<isStaticVector_ && staticSize_ >= 4, U>>
    auto& w() const {
        return data_[3];
    }

    auto operator()(size_type row, size_type column) const { return data_[index(row, column)]; }

    auto begin() const { return std::begin(data_); }

    auto end() const { return std::end(data_); }

    auto rows() const { return rows_; }

    auto columns() const { return columns_; }

    auto closeTo(const Matrix& other, T epsilon) {
        return rows_ == other.rows_ && columns_ == other.columns_ &&
               std::equal(begin(), end(), other.begin(), other.end(),
                          [epsilon](auto a, auto b) { return dansandu::eyecandy::math::closeTo(a, b, epsilon); });
    }

private:
    auto index(size_type row, size_type column) const {
        if (row < 0 || row >= rows_ || column < 0 || column >= columns_)
            THROW(std::out_of_range, "cannot index the ", row, ", ", column, " element in a ", rows_, "x", columns_,
                  " matrix -- indices are out of bounds");

        return row * columns_ + column;
    }

    size_type rows_;
    size_type columns_;
    std::vector<value_type> data_;
};
}
}
}
