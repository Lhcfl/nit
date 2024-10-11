#include "./nit_types.hpp"
#include <functional>
// #include <vector>

#ifndef LINCA_NIT_UTILS
#define LINCA_NIT_UTILS

namespace Utils {

template <typename T> class Some {
  std::function<bool(T)> fn;

public:
  explicit Some(std::function<bool(const T &)> &&function) : fn(function) {}

  template <typename Iterable>
  friend bool operator|(const Iterable &arr, const Some &me) {
    for (auto &t : arr) {
      if (me.fn(t))
        return true;
    }
    return false;
  }
};
} // namespace Utils

#endif