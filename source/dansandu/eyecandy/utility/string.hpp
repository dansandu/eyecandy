#pragma once

#include <algorithm>
#include <sstream>
#include <stdexcept>
#include <string>

namespace dansandu {
namespace eyecandy {
namespace utility {

template<typename I>
auto join(const I& iterable, const std::string& separator) {
    std::stringstream ss;
    for (const auto& element : iterable)
        ss << element << separator;
    auto result = ss.str();
    result.erase(result.end() - std::min(separator.size(), result.size()), result.end());
    return result;
}

inline auto formatWork(std::ostream&) {}

template<typename A, typename... AA>
auto formatWork(std::ostream& buffer, A&& argument, AA&&... arguments) {
    buffer << std::forward<A>(argument);
    formatWork(buffer, std::forward<AA>(arguments)...);
}

template<typename... AA>
auto format(AA&&... arguments) {
    std::stringstream buffer;
    formatWork(buffer, std::forward<AA>(arguments)...);
    return buffer.str();
}
}
}
}
