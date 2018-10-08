#pragma once

namespace dansandu {
namespace eyecandy {
namespace math {

enum class StorageStrategy { stack, heap };

constexpr auto dynamic = -1;

using size_type = int;

template<typename T, size_type rows, size_type columns>
class recommended_storage_strategy_for {
    static_assert(rows > 0 || rows == dynamic, "matrix rows must be positive or dynamic");
    static_assert(columns > 0 || columns == dynamic, "matrix columns must be positive or dynamic");
    constexpr static size_type maximumStackBytes{32};

public:
    constexpr static auto value =
        rows == dynamic || columns == dynamic || sizeof(T) * rows * columns > maximumStackBytes
            ? StorageStrategy::heap
            : StorageStrategy::stack;
};

template<typename T, size_type rows, size_type columns,
         StorageStrategy storageStrategy = recommended_storage_strategy_for<T, rows, columns>::value>
class Matrix;
}
}
}
