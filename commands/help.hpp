#include "commands/base.hpp"
#include "config/config.h"
#include "lib/nit_common.hpp"
#include "models/commit.hpp"
#include <string>

#ifndef __H_NIT_COMMAND_HELP
#define __H_NIT_COMMAND_HELP

namespace NitCommand {
class Help : public Base {
public:
  Help() : Base() {}

  void exec() const {
    NitLogger logger;
    logger.log("========= Nit Help ==========");
    logger.log("Nit version", NitProjectConfig::VERSION);
    logger.log("");
    logger.log("Commands:");
    logger.log("  init");
    logger.log("  commit [msg]");
  }
};

} // namespace NitCommand

#endif