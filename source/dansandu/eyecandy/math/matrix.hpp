#pragma once

#include "dansandu/eyecandy/math/matrix.hpp"
#include "dansandu/eyecandy/math/numeric_traits.hpp"

#include <algorithm>
#include <stdexcept>
#include <vector>

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

    Matrix(size_type rows, size_type columns, value_type fillValue = addition_identity<value_type>)
        : rows_{rows}, columns_{columns} {
        if (rows < 0 || columns < 0 || (rows != 0) != (columns != 0))
            throw std::runtime_error("matrix size must be greater or equal than zero");

        data_ = std::vector<value_type>(rows_ * columns_, fillValue);
    }

    Matrix(std::initializer_list<std::initializer_list<value_type>> list) {
        if ((rows_ = list.size()) != 0)
            columns_ = list.begin()->size();
        else
            columns_ = 0;

        for (auto row : list) {
            data_.insert(data_.end(), row.begin(), row.end());
            if (columns_ != static_cast<size_type>(row.size()))
                throw std::runtime_error("columns must be the same size");
        }
    }

    value_type& operator()(size_type row, size_type column) noexcept { return data_[columns_ * row + column]; }

    value_type operator()(const size_type row, const size_type column) const noexcept {
        return data_[columns_ * row + column];
    }

    Matrix& operator=(const Matrix& rhs) = default;

    Matrix& operator=(Matrix&& rhs) noexcept {
        if (this != &rhs) {
            rows_ = rhs.rows_;
            columns_ = rhs.columns_;
            data_ = std::move(rhs.data_);
            rhs.rows_ = rhs.columns_ = 0;
        }
    }

    Matrix& operator*=(const Matrix& rhs);

    size_type rows() const noexcept { return rows_; }

    size_type columns() const noexcept { return columns_; }

    bool closeTo(const Matrix& rhs, value_type epsilon) const {
        return rows_ == rhs.rows_ && columns_ == rhs.columns_ &&
               std::equal(data_.begin(), data_.end(), rhs.data_.begin(), rhs.data_.end(),
                          [epsilon](auto a, auto b) { return dansandu::eyecandy::math::closeTo(a, b, epsilon); });
    }

private:
    size_type rows_;
    size_type columns_;
    std::vector<value_type> data_;
};

template<typename T>
Matrix<T> operator*(const Matrix<T>& lhs, const Matrix<T>& rhs) {
    if (lhs.columns() != rhs.rows())
        throw std::runtime_error("left hand side matrix columns do not match right hand side matrix rows");

    Matrix<T> result(lhs.rows(), rhs.columns());
    for (auto i = 0; i < lhs.rows(); ++i)
        for (auto k = 0; k < rhs.columns(); ++k)
            for (auto j = 0; j < lhs.columns(); ++j)
                result(i, k) += lhs(i, j) * rhs(j, k);
    return result;
}

template<typename T>
Matrix<T>& Matrix<T>::operator*=(const Matrix<T>& rhs) {
    return *this = *this * rhs;
}
}
}
}
