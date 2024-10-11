#include <string>

#ifndef NIT_COLORS_HPP
#define NIT_COLORS_HPP
namespace NitColor {

static const std::string SIGN_NORMAL = "\x1b[0m";
static const std::string SIGN_RED = "\x1b[31m";
static const std::string SIGN_GREEN = "\x1b[32m";
static const std::string SIGN_YELLOW = "\x1b[33m";
static const std::string SIGN_BLUE = "\x1b[34m";
static const std::string SIGN_WHITE = "\x1b[37m";

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