#pragma once

#include <sstream>

namespace dansandu {
namespace eyecandy {
namespace utility {

template<typename I>
std::string join(const I& iterable, const std::string& separator) {
    std::stringstream ss;
    for (const auto& element : iterable)
        ss << element << separator;
    auto result = ss.str();
    result.erase(result.end() - std::min(separator.size(), result.size()), result.end());
    return result;
}
}
}
}
