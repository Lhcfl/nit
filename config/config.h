#ifndef LINCA_CONFIG_H
#define LINCA_CONFIG_H

#include <string>
namespace NitProjectConfig {

static const std::string VERSION = "0.0.1";

enum class NIT_LOG_LEVELS {
  DEBUG = 0,
  INFO = 1,
  WARN = 2,
  ERROR = 3,
  DISABLED = 99999,
};
static constexpr NIT_LOG_LEVELS NIT_LOG_LEVEL = NIT_LOG_LEVELS::DEBUG;

} // namespace NitProjectConfig
#endif