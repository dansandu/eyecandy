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

inline auto formatWork(std::stringstream& buffer, typename std::string::const_iterator begin,
                       typename std::string::const_iterator end) {
    if (std::find(begin, end, '#') != end)
        throw std::invalid_argument{"too few arguments passed to format"};

    std::copy(begin, end, std::ostream_iterator<typename std::string::value_type>(buffer));
}

template<typename A, typename... AA>
auto formatWork(std::stringstream& buffer, typename std::string::const_iterator begin,
                typename std::string::const_iterator end, A&& argument, AA&&... arguments) {
    auto position = std::find(begin, end, '#');
    if (position == end)
        throw std::invalid_argument{"too many arguments passed to format"};

    std::copy(begin, position, std::ostream_iterator<typename std::string::value_type>(buffer));
    buffer << argument;
    formatWork(buffer, position + 1, end, std::forward<AA>(arguments)...);
}

template<typename... AA>
std::string format(const std::string& pattern, AA&&... arguments) {
    std::stringstream buffer;
    formatWork(buffer, pattern.begin(), pattern.end(), std::forward<AA>(arguments)...);
    return buffer.str();
}
}
}
}
