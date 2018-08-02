#pragma once

#include "dansandu/eyecandy/math/skip_iterator.hpp"

namespace dansandu {
namespace eyecandy {
namespace math {

template<typename I, typename S, typename L>
class MatrixView {
public:
    MatrixView(I begin, I end, S step, L dereference)
        : beginPrototype_{begin, end, step, dereference}, endPrototype_{end, end, step, dereference} {}

    auto begin() const { return beginPrototype_; }

    auto end() const { return endPrototype_; }

    auto operator()(S index) const { return *(beginPrototype_ + index); }

private:
    SkipIterator<I, S, L> beginPrototype_;
    SkipIterator<I, S, L> endPrototype_;
};

template<typename I, typename S, typename L>
auto makeMatrixView(I begin, I end, S step, L dereference) {
    return MatrixView<I, S, L>{begin, end, step, dereference};
}

template<typename I, typename S>
auto rowView(I beginning, S rows, S columns) {
    return makeMatrixView(beginning, beginning + rows * columns, columns, [rows, columns](auto position) {
        return makeMatrixView(position, position + columns, 1, [](auto position) { return *position; });
    });
}

template<typename I, typename S>
auto columnView(I beginning, S rows, S columns) {
    return makeMatrixView(beginning, beginning + columns, 1, [rows, columns](auto position) {
        return makeMatrixView(position, position + rows * columns, columns, [](auto position) { return *position; });
    });
}
}
}
}
