#pragma once

#include "dansandu/math/matrix.hpp"

namespace dansandu::eyecandy
{

using Point2 = dansandu::math::matrix::Matrix<int, 1, 2>;

using Matrix4 = dansandu::math::matrix::Matrix<float, 4, 4>;

using Vector3 = dansandu::math::matrix::Matrix<float, 1, 3>;

using Vector3View = dansandu::math::matrix::MatrixView<float, 1, 3>;

using ConstantVector3View = dansandu::math::matrix::ConstantMatrixView<float, 1, 3>;

}
