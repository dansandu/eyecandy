#pragma once

#include "dansandu/eyecandy/math/matrix.hpp"
#include "dansandu/eyecandy/math/numeric_traits.hpp"

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
}
}
}