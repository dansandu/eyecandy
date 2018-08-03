#pragma once

#include "dansandu/eyecandy/math/matrix_view.hpp"
#include "dansandu/eyecandy/math/numeric_traits.hpp"

#include <algorithm>
#include <iomanip>
#include <sstream>
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

    Matrix() : rowCount_{0}, columnCount_{0} {}

    Matrix(const Matrix& rhs) = default;

    Matrix(Matrix&& rhs) noexcept
        : rowCount_{rhs.rowCount_}, columnCount_{rhs.columnCount_}, data_{std::move(rhs.data_)} {
        rhs.rowCount_ = rhs.columnCount_ = 0;
    }

    Matrix(size_type rowCount, size_type columnCount, value_type fillValue = additive_identity<value_type>)
        : rowCount_{rowCount}, columnCount_{columnCount} {
        if (rowCount < 0 || columnCount < 0 || (rowCount != 0) != (columnCount != 0))
            throw std::runtime_error("matrix size must be greater or equal than zero");

        data_ = std::vector<value_type>(rowCount_ * columnCount_, fillValue);
    }

    Matrix(std::initializer_list<std::initializer_list<value_type>> list)
        : rowCount_{static_cast<size_type>(list.size())},
          columnCount_{rowCount_ ? static_cast<size_type>(list.begin()->size()) : 0} {
        for (auto row : list) {
            if (columnCount_ != static_cast<size_type>(row.size()))
                throw std::runtime_error("columnCount must be the same size");

            data_.insert(data_.end(), row.begin(), row.end());
        }
    }

    value_type& operator()(size_type row, size_type column) noexcept { return data_[index(row, column)]; }

    value_type operator()(const size_type row, const size_type column) const noexcept {
        return data_[index(row, column)];
    }

    void pushRow(const std::vector<value_type>& row) {
        if (columnCount_ != static_cast<size_type>(row.size()) & columnCount_)
            throw std::invalid_argument("row size must match matrix columnCount");

        data_.insert(data_.end(), row.begin(), row.end());
        ++rowCount_;
        columnCount_ = row.size();
    }

    Matrix& operator=(const Matrix& rhs) = default;

    Matrix& operator=(Matrix&& rhs) noexcept {
        if (this != &rhs) {
            rowCount_ = rhs.rowCount_;
            columnCount_ = rhs.columnCount_;
            data_ = std::move(rhs.data_);
            rhs.rowCount_ = rhs.columnCount_ = 0;
        }
        return *this;
    }

    Matrix& operator*=(const Matrix& rhs) { return *this = *this * rhs; }

    size_type rowCount() const noexcept { return rowCount_; }

    size_type columnCount() const noexcept { return columnCount_; }

    auto rows() const { return rowView(data_.begin(), rowCount_, columnCount_); }

    auto columns() const { return columnView(data_.begin(), rowCount_, columnCount_); }

    bool closeTo(const Matrix& rhs, value_type epsilon) const {
        return rowCount_ == rhs.rowCount_ && columnCount_ == rhs.columnCount_ &&
               std::equal(data_.begin(), data_.end(), rhs.data_.begin(), rhs.data_.end(),
                          [epsilon](auto a, auto b) { return dansandu::eyecandy::math::closeTo(a, b, epsilon); });
    }

    std::string toString() const {
        std::stringstream ss;
        ss << std::fixed << std::setprecision(2) << *this;
        return ss.str();
    }

private:
    size_type index(size_type row, size_type column) const { return row * columnCount_ + column; }

    size_type rowCount_;
    size_type columnCount_;
    std::vector<value_type> data_;
};

template<typename T>
Matrix<T> operator*(const Matrix<T>& lhs, const Matrix<T>& rhs) {
    if (lhs.columnCount() != rhs.rowCount())
        throw std::runtime_error("left hand side matrix columnCount do not match right hand side matrix rowCount");

    Matrix<T> result(lhs.rowCount(), rhs.columnCount());
    for (auto i = 0; i < lhs.rowCount(); ++i)
        for (auto k = 0; k < rhs.columnCount(); ++k)
            for (auto j = 0; j < lhs.columnCount(); ++j)
                result(i, k) += lhs(i, j) * rhs(j, k);
    return result;
}

template<typename T>
std::ostream& operator<<(std::ostream& os, const Matrix<T>& matrix) {
    for (auto row : matrix.rows()) {
        for (auto element : row)
            os << element << " ";
        os << std::endl;
    }
    return os;
}
}
}
}
