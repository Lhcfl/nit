#include <type_traits>

#pragma once

namespace NitTypeUtils {

template <typename T, typename = void>
struct has_begin_end : std::false_type {};

template <typename T>
struct has_begin_end<T, std::void_t<decltype(std::declval<T>().begin()),
                                    decltype(std::declval<T>().end())>>
    : std::true_type {};

} // namespace NitTypeUtils
