#pragma once

#include "dansandu/eyecandy/math/numeric_traits.hpp"

#include <algorithm>
#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <vector>

#include <iostream>

namespace dansandu {
namespace eyecandy {
namespace math {

template<typename T>
class Matrix {
public:
    using size_type = int;
    using value_type = T;

    Matrix() : rows_{0}, columns_{0} {}

    Matrix(const Matrix& rhs) = default;

    Matrix(Matrix&& rhs) noexcept : rows_{rhs.rows_}, columns_{rhs.columns_}, data_{std::move(rhs.data_)} {
        rhs.rows_ = rhs.columns_ = 0;
    }

    Matrix(size_type rowCount, size_type columnCount, value_type fillValue = additive_identity<value_type>)
        : rows_{rowCount}, columns_{columnCount} {
        if (rows_ < 0 || columns_ < 0 || (rows_ != 0) != (columns_ != 0))
            throw std::invalid_argument{"matrix size must be greater or equal than zero"};
        data_ = std::vector<value_type>(rows_ * columns_, fillValue);
    }

    Matrix(std::initializer_list<value_type> column)
        : rows_{static_cast<size_type>(column.size())}, columns_{1}, data_(column) {}

    Matrix(std::initializer_list<std::initializer_list<value_type>> list)
        : rows_{static_cast<size_type>(list.size())},
          columns_{rows_ ? static_cast<size_type>(list.begin()->size()) : 0} {
        for (auto row : list) {
            if (columns_ != static_cast<size_type>(row.size()))
                throw std::invalid_argument{"columnCount must be the same size"};
            data_.insert(data_.end(), row.begin(), row.end());
        }
    }

    Matrix<T>& operator=(const Matrix& rhs) = default;

    auto& operator=(Matrix&& rhs) noexcept {
        if (this != &rhs) {
            rows_ = rhs.rows_;
            columns_ = rhs.columns_;
            data_ = std::move(rhs.data_);
            rhs.rows_ = rhs.columns_ = 0;
        }
        return *this;
    }

    auto& operator*=(const Matrix& rhs) { return *this = *this * rhs; }

    auto& operator*=(value_type scalar) {
        std::transform(data_.begin(), data_.end(), data_.begin(), [scalar](auto a) { return a * scalar; });
        return *this;
    }

    auto& operator-=(const Matrix& other) {
        if (rows_ != other.rows_ || columns_ != other.columns_)
            throw std::invalid_argument{"matrix dimensions must match"};

        std::transform(data_.begin(), data_.end(), other.data_.begin(), data_.begin(),
                       [](auto a, auto b) { return a - b; });
        return *this;
    }

    auto& operator()(size_type n) { return data_[index(n)]; }

    auto& operator()(size_type row, size_type column) { return data_[index(row, column)]; }

    auto swap(Matrix& other) noexcept {
        using std::swap;
        swap(rows_, other.rows_);
        swap(columns_, other.columns_);
        swap(data_, other.data_);
    }

    auto operator()(size_type n) const { return data_[index(n)]; }

    auto operator()(const size_type row, const size_type column) const { return data_[index(row, column)]; }

    auto rows() const { return rows_; }

    auto columns() const { return columns_; }

    auto closeTo(const Matrix& rhs, value_type epsilon) const {
        return rows_ == rhs.rows_ && columns_ == rhs.columns_ &&
               std::equal(data_.begin(), data_.end(), rhs.data_.begin(), rhs.data_.end(),
                          [epsilon](auto a, auto b) { return dansandu::eyecandy::math::closeTo(a, b, epsilon); });
    }

    auto toString() const {
        std::stringstream ss;
        ss << std::fixed << std::setprecision(2) << *this;
        return ss.str();
    }

    auto begin() const { return data_.begin(); }

    auto end() const { return data_.end(); }

private:
    auto index(size_type n) const {
        if (rows_ != 1 && columns_ != 1)
            throw std::runtime_error{"must single index into a row or column matrix"};

        if (n < 0 || n >= static_cast<size_type>(data_.size()))
            throw std::out_of_range{"matrix indexing is out of bound"};

        return n;
    }

    auto index(size_type row, size_type column) const {
        if (row < 0 || row >= rows_ || column < 0 || column >= columns_)
            throw std::out_of_range{"matrix indexing is out of bounds"};

        return row * columns_ + column;
    }

    size_type rows_;
    size_type columns_;
    std::vector<value_type> data_;
};

template<typename T>
auto operator*(const Matrix<T>& lhs, const Matrix<T>& rhs) {
    if (lhs.columns() != rhs.rows())
        throw std::invalid_argument{"left hand side matrix columns do not match right hand side matrix rows"};

    Matrix<T> result(lhs.rows(), rhs.columns());
    for (auto i = 0; i < lhs.rows(); ++i)
        for (auto k = 0; k < rhs.columns(); ++k)
            for (auto j = 0; j < lhs.columns(); ++j)
                result(i, k) += lhs(i, j) * rhs(j, k);
    return result;
}

template<typename T>
auto operator-(const Matrix<T>& lhs, const Matrix<T>& rhs) {
    auto result = lhs;
    return result -= rhs;
}

template<typename T>
auto& operator<<(std::ostream& os, const Matrix<T>& matrix) {
    for (auto i = 0; i < matrix.rows(); ++i) {
        auto j = 0;
        while (j + 1 < matrix.columns())
            os << matrix(i, j++) << ", ";
        os << matrix(i, j) << std::endl;
    }
    return os;
}

template<typename T>
auto swap(Matrix<T>& lhs, Matrix<T>& rhs) {
    lhs.swap(rhs);
}

template<typename T>
auto magnitude(const Matrix<T>& matrix) {
    if (matrix.rows() != 1 && matrix.columns() != 1)
        throw std::invalid_argument{"matrix must be a row or column vector"};

    auto result = additive_identity<T>;
    for (auto element : matrix)
        result += element * element;
    return std::sqrt(result);
}

template<typename T>
auto operator*(const Matrix<T>& matrix, T scalar) {
    Matrix<T> result = matrix;
    return result *= scalar;
}

template<typename T>
auto operator*(T scalar, const Matrix<T>& matrix) {
    Matrix<T> result = matrix;
    return result *= scalar;
}

template<typename T>
auto normalize(const Matrix<T>& matrix) {
    return (multiplicative_identity<T> / magnitude(matrix)) * matrix;
}

template<typename T>
auto composeWork(Matrix<T>&, int, int, int) {}

template<typename T, typename... AA>
auto composeWork(Matrix<T>& result, int xOffset, int yOffset, int lastColumnSize, const Matrix<T>& matrix,
                 AA&&... args) {
    if (xOffset + matrix.columns() <= result.columns() && yOffset + matrix.rows() <= result.rows() ||
        lastColumnSize != -1 && lastColumnSize != matrix.rows()) {
        lastColumnSize = matrix.rows();
        for (auto y = 0; y < matrix.rows(); ++y)
            for (auto x = 0; x < matrix.columns(); ++x)
                result(xOffset + x, yOffset + y) = matrix(x, y);
        xOffset += matrix.columns();
        if (xOffset == result.columns()) {
            yOffset += matrix.rows();
            if (yOffset < result.rows()) {
                xOffset = 0;
                lastColumnSize = -1;
            }
        }
    } else
        throw std::invalid_argument{"failed to compose elements into a matrix"};

    composeWork(result, xOffset, yOffset, lastColumnSize, std::forward<AA>(args)...);
}

template<typename T, typename... AA>
auto composeWork(Matrix<T>& result, int xOffset, int yOffset, int lastColumnSize, T scalar, AA&&... args) {
    if (xOffset + 1 <= result.columns() && yOffset + 1 <= result.rows() ||
        lastColumnSize != -1 && lastColumnSize != 1) {
        lastColumnSize = 1;
        result(xOffset, yOffset) = scalar;
        ++xOffset;
        if (xOffset == result.columns()) {
            ++yOffset;
            if (yOffset < result.rows()) {
                xOffset = 0;
                lastColumnSize = -1;
            }
        }
    } else
        throw std::invalid_argument{"failed to compose elements into a matrix"};

    composeWork(result, xOffset, yOffset, lastColumnSize, std::forward<AA>(args)...);
}

template<typename T, typename... AA>
auto compose(int rows, int columns, AA&&... args) {
    Matrix<T> result(rows, columns);
    composeWork(result, 0, 0, -1, std::forward<AA>(args)...);
    return result;
}

template<typename T>
auto crossProduct(const Matrix<T>& lhs, const Matrix<T>& rhs) {
    if (lhs.rows() != 1 && lhs.columns() != 1)
        throw std::invalid_argument{"left hand side must be a row or column vector"};

    if (rhs.rows() != 1 && rhs.columns() != 1)
        throw std::invalid_argument{"right hand side must be a row or column vector"};

    if (lhs.rows() * lhs.columns() != 3 || rhs.rows() * rhs.columns() != 3)
        throw std::invalid_argument{"vectors must have the length equal to 3"};

    // clang-format off
    return Matrix<T>{lhs(1) * rhs(2) - rhs(1) * lhs(2),
                     rhs(0) * lhs(2) - lhs(0) * rhs(2),
                     lhs(0) * rhs(1) - lhs(1) * rhs(0)};
    // clang-format on
}

template<typename T>
auto dehomogenize(Matrix<T>& matrix) {
    for (auto i = 0; i + 1 < matrix.rows(); ++i)
        for (auto j = 0; j < matrix.columns(); ++j)
            matrix(i, j) = matrix(i, j) / matrix(matrix.rows() - 1, j);

    for (auto j = 0; j < matrix.columns(); ++j)
        matrix(matrix.rows() - 1, j) = 1.0;
}
}
}
}
