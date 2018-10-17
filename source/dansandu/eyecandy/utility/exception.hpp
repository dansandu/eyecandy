#pragma once

#include "dansandu/eyecandy/utility/string.hpp"

#include <stdexcept>
#include <thread>

namespace dansandu {
namespace eyecandy {
namespace utility {

template<int line, typename E, typename... AA>
auto prettyThrow(const char* exception, const char* function, const char* file, const std::string& message,
                 AA&&... arguments) {
    throw E{format("'", exception, "' exception in thread '", std::this_thread::get_id(),
                   "': ", format(message, std::forward<AA>(arguments)...), std::endl, "    at ", function, "(", file,
                   ":", line, ")")};
}
}
}
}

#define THROW(exception, args...)                                                                                      \
    dansandu::eyecandy::utility::prettyThrow<__LINE__, exception>(#exception, __func__, __FILE__, args)
