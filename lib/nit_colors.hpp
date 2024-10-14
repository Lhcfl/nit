#include <string>

#ifndef __H_NIT_COLORS_HPP
#define __H_NIT_COLORS_HPP
namespace NitColor {

static constexpr std::string SIGN_NORMAL = "\x1b[0m";
static constexpr std::string SIGN_RED = "\x1b[31m";
static constexpr std::string SIGN_GREEN = "\x1b[32m";
static constexpr std::string SIGN_YELLOW = "\x1b[33m";
static constexpr std::string SIGN_BLUE = "\x1b[34m";
static constexpr std::string SIGN_WHITE = "\x1b[37m";

inline std::string red(const std::string &s) {
  return SIGN_RED + s + SIGN_NORMAL;
}
inline std::string green(const std::string &s) {
  return SIGN_GREEN + s + SIGN_NORMAL;
}
inline std::string yellow(const std::string &s) {
  return SIGN_YELLOW + s + SIGN_NORMAL;
}
inline std::string blue(const std::string &s) {
  return SIGN_BLUE + s + SIGN_NORMAL;
}
inline std::string white(const std::string &s) {
  return SIGN_WHITE + s + SIGN_NORMAL;
}

} // namespace NitColor
#endif