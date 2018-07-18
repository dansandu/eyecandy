#pragma once

#include "dansandu/eyecandy/math/matrix.hpp"
#include "dansandu/eyecandy/math/numeric_traits.hpp"

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
    using iterator = typename std::vector<value_type>::iterator;
    using const_iterator = typename std::vector<value_type>::const_iterator;

    Matrix() : rows_{0}, columns_{0} {}

    Matrix(const Matrix& input) = default;

    Matrix(Matrix&& input) noexcept = default;

    Matrix(size_type rows, size_type columns, value_type fillValue = addition_identity<value_type>)
        : rows_{rows}, columns_{columns} {
        if (rows < 0 || columns < 0 || (rows != 0) != (columns != 0))
            throw std::runtime_error("matrix size must be greater or equal than zero");

        data_ = std::vector<value_type>(rows_ * columns_, fillValue);
    }

    Matrix(std::initializer_list<std::initializer_list<value_type>> input) {
        if ((rows_ = input.size()) != 0)
            columns_ = input.begin()->size();
        else
            columns_ = 0;

        for (auto row : input) {
            data_.insert(data_.end(), row.begin(), row.end());
            if (columns_ != static_cast<size_type>(row.size()))
                throw std::runtime_error("columns must be the same size");
        }
    }

    size_type rows() const noexcept { return rows_; }

    size_type columns() const noexcept { return columns_; }

    value_type& operator()(size_type row, size_type column) noexcept { return data_[columns_ * row + column]; }

    value_type operator()(const size_type row, const size_type column) const noexcept {
        return data_[columns_ * row + column];
    }

    Matrix& operator=(const Matrix& rhs) = default;

    Matrix& operator=(Matrix&& rhs) noexcept = default;

    Matrix& operator*=(const Matrix& rhs);

    iterator begin() { return data_.begin(); }

    iterator end() { return data_.end(); }

    const_iterator cbegin() const { return data_.cbegin(); }

    const_iterator cend() const { return data_.cend(); }

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
