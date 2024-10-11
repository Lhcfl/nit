#include "commands/base.hpp"
#include "config/config.h"
#include "lib/nit_common.hpp"
#include "models/commit.hpp"
#include <string>

#ifndef NIT_COMMAND_HELP
#define NIT_COMMAND_HELP

namespace NitCommand {
class Help : public Base {
public:
  static const int ARG_NUMBER = 1;
  static const std::string NAME;

  Help() : Base() {}

  void exec() const {
    NitLogger logger;
    logger.log("========= Nit Help ==========");
    logger.log("Nit version", NitProjectConfig::VERSION);
    logger.log("");
    logger.log("Commands:");
    logger.log("  commit [msg]");
  }
};

} // namespace NitCommand

#endif