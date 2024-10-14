#include <cctype>
#include <functional>
#include <string>
#include <string_view>
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

inline bool isSpace(const char c) { return c == ' ' || c == '\n' || c == '\t'; }

inline std::string_view rtrim(const std::string &s) {
  auto it = s.rbegin();
  while (it != s.rend() && isSpace(*it))
    it++;
  return std::string_view(s.begin(), it.base());
}

} // namespace Utils
#endif