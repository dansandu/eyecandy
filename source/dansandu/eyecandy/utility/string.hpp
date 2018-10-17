#pragma once

#include <algorithm>
#include <iterator>
#include <sstream>
#include <stdexcept>
#include <string>

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

inline auto formatWork(std::ostream& buffer) {}

template<typename A, typename... AA>
auto formatWork(std::ostream& buffer, A&& argument, AA&&... arguments) {
    buffer << argument;
    formatWork(buffer, std::forward<AA>(arguments)...);
}

template<typename... AA>
std::string format(AA&&... arguments) {
    std::stringstream buffer;
    formatWork(buffer, std::forward<AA>(arguments)...);
    return buffer.str();
}
}
}
}
