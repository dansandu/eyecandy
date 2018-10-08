#pragma once

#include "dansandu/eyecandy/math/base_matrix.hpp"
#include "dansandu/eyecandy/math/heap_matrix.hpp"
#include "dansandu/eyecandy/math/stack_matrix.hpp"

namespace dansandu {
namespace eyecandy {
namespace math {

template<size_type staticRowBegin, size_type staticRowCount, size_type staticColumnBegin, size_type staticColumnCount>
struct slicer {
    template<typename T, size_type staticRows, size_type staticColumns,
             typename = std::enable_if_t<
                 staticRowBegin != dynamic && staticRowCount != dynamic && staticColumnBegin != dynamic &&
                     staticColumnCount != dynamic &&
                     ((staticRows != dynamic &&
                       subinterval(staticRowBegin, staticRowBegin + staticRowCount, 0, staticRows)) ||
                      (staticRows == dynamic && staticRowBegin >= 0 && staticRowCount >= 1)) &&
                     ((staticColumns != dynamic &&
                       subinterval(staticColumnBegin, staticColumnBegin + staticColumnCount, 0, staticColumns)) ||
                      (staticColumns == dynamic && staticColumnBegin >= 0 && staticColumnCount >= 1)),
                 T>>
    static auto slice(const Matrix<T, staticRows, staticColumns>& matrix) {
        if (!subinterval(staticRowBegin, staticRowBegin + staticRowCount, 0, matrix.rows()))
            THROW(std::invalid_argument, "cannot slice rows [#, #) of a #x# matrix -- slice exceeds matrix boundries",
                  staticRowBegin, staticRowBegin + staticRowCount, matrix.rows(), matrix.columns());

        if (!subinterval(staticColumnBegin, staticColumnBegin + staticColumnCount, 0, matrix.columns()))
            THROW(std::invalid_argument,
                  "cannot slice columns [#, #) of a #x# matrix -- slice exceeds matrix boundries", staticColumnBegin,
                  staticColumnBegin + staticColumnCount, matrix.rows(), matrix.columns());

        Matrix<T, staticRowCount, staticColumnCount> result{staticRowCount, staticColumnCount};
        for (auto i = 0; i < staticRowCount; ++i)
            for (auto j = 0; j < staticColumnCount; ++j)
                result(i, j) = matrix(staticRowBegin + i, staticColumnBegin + j);
        return result;
    }
};

template<size_type staticRowCount, size_type staticColumnBegin, size_type staticColumnCount>
struct slicer<dynamic, staticRowCount, staticColumnBegin, staticColumnCount> {
    template<typename T, size_type staticRows, size_type staticColumns,
             typename = std::enable_if_t<
                 ((staticRows != dynamic && interval(staticRowCount, 1, staticRows + 1)) ||
                  (staticRows == dynamic && staticRowCount >= 1)) &&
                     ((staticColumns != dynamic &&
                       subinterval(staticColumnBegin, staticColumnBegin + staticColumnCount, 0, staticColumns)) ||
                      (staticColumns == dynamic && staticColumnBegin >= 0 && staticColumnCount >= 1)),
                 T>>
    static auto slice(const Matrix<T, staticRows, staticColumns>& matrix, size_type dynamicRowBegin) {
        if (!subinterval(dynamicRowBegin, dynamicRowBegin + staticRowCount, 0, matrix.rows()))
            THROW(std::invalid_argument, "cannot slice rows [#, #) of a #x# matrix -- slice exceeds matrix boundries",
                  dynamicRowBegin, dynamicRowBegin + staticRowCount, matrix.rows(), matrix.columns());

        if (!subinterval(staticColumnBegin, staticColumnBegin + staticColumnCount, 0, matrix.columns()))
            THROW(std::invalid_argument,
                  "cannot slice columns [#, #) of a #x# matrix -- slice exceeds matrix boundries", staticColumnBegin,
                  staticColumnBegin + staticColumnCount, matrix.rows(), matrix.columns());

        Matrix<T, staticRowCount, staticColumnCount> result{staticRowCount, staticColumnCount};
        for (auto i = 0; i < staticRowCount; ++i)
            for (auto j = 0; j < staticColumnCount; ++j)
                result(i, j) = matrix(dynamicRowBegin + i, staticColumnBegin + j);
        return result;
    }
};

template<size_type staticRowBegin, size_type staticColumnBegin, size_type staticColumnCount>
struct slicer<staticRowBegin, dynamic, staticColumnBegin, staticColumnCount> {
    template<typename T, size_type staticRows, size_type staticColumns,
             typename = std::enable_if_t<
                 ((staticRows != dynamic && interval(staticRowBegin, 0, staticRows)) ||
                  (staticRows == dynamic && staticRowBegin >= 0)) &&
                     ((staticColumns != dynamic &&
                       subinterval(staticColumnBegin, staticColumnBegin + staticColumnCount, 0, staticColumns)) ||
                      (staticColumns == dynamic && staticColumnBegin >= 0 && staticColumnCount >= 1)),
                 T>>
    static auto slice(const Matrix<T, staticRows, staticColumns>& matrix, size_type dynamicRowCount) {
        if (!subinterval(staticRowBegin, staticRowBegin + dynamicRowCount, 0, matrix.rows()))
            THROW(std::invalid_argument, "cannot slice rows [#, #) of a #x# matrix -- slice exceeds matrix boundries",
                  staticRowBegin, staticRowBegin + dynamicRowCount, matrix.rows(), matrix.columns());

        if (!subinterval(staticColumnBegin, staticColumnBegin + staticColumnCount, 0, matrix.columns()))
            THROW(std::invalid_argument,
                  "cannot slice columns [#, #) of a #x# matrix -- slice exceeds matrix boundries", staticColumnBegin,
                  staticColumnBegin + staticColumnCount, matrix.rows(), matrix.columns());

        Matrix<T, dynamic, staticColumnCount> result{dynamicRowCount, staticColumnCount};
        for (auto i = 0; i < dynamicRowCount; ++i)
            for (auto j = 0; j < staticColumnCount; ++j)
                result(i, j) = matrix(staticRowBegin + i, staticColumnBegin + j);
        return result;
    }
};
}
}
}
