#pragma once

#include "dansandu/eyecandy/utility/string.hpp"

#include <stdexcept>
#include <thread>

namespace dansandu {
namespace eyecandy {
namespace utility {

template<typename E, typename... AA>
auto prettyThrow(const char* exception, const char* function, const char* file, int line, AA&&... arguments) {
    auto message = format(std::forward<AA>(arguments)...);
    throw E{format("'", exception, "' exception in thread '", std::this_thread::get_id(), "': ", message, "\n    at ",
                   function, "(", file, ":", line, ")")};
}
}
}
}

#define THROW(exception, args...)                                                                                      \
    dansandu::eyecandy::utility::prettyThrow<exception>(#exception, __func__, __FILE__, __LINE__, args)
